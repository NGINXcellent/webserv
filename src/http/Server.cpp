/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:22:33 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/03 21:41:56 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Server.hpp"

#include <iostream> // cout
#include <fstream>
#include <sstream>  // stringstream
#include <algorithm>

#include "../../include/http/HttpTime.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/HttpResponseComposer.hpp"
#include "../../include/http/MimeType.hpp"
#include "../../include/io/TcpServerSocket.hpp"
#include "../../include/io/FileSystem.hpp"
#include "../../include/io/FileReader.hpp"
#include "../../include/http/HttpStatus.hpp"

Server::Server(const struct s_serverConfig& config) {
  port = strtol(config.port.c_str(), NULL, 0);
  host = config.host;
  server_name = config.server_name;
  srv_max_body_size = config.srv_max_body_size;
  error_pages = config.error_page;
  locations = config.location;
}

Server::~Server(void) {
  delete socket;
}

HttpStatusCode Server::resolve(HttpRequest *request, HttpResponse *response) {
  std::string uTimestamp = request->getUnmodifiedSinceTimestamp();

  if (!uTimestamp.empty() && \
      HttpTime::isModifiedSince(uTimestamp, request->getResource())) {
    // shim
    return (Precondition_Failed);
  }

  std::string requestMethod = request->getMethod();
  std::vector<std::string> allowedMethods = request->getAllowedMethods();
  std::vector<std::string>::iterator it;
  it = std::find(allowedMethods.begin(), allowedMethods.end(), requestMethod);

  if (!allowedMethods.empty() && it == allowedMethods.end()) {
    return (Method_Not_Allowed);
  }

  HttpStatusCode opStatus = Ready; 

  if (requestMethod == "GET")
    opStatus = get(request, response);
  else if (requestMethod == "DELETE")
    opStatus = del(request, response);
  else if (requestMethod == "POST")
    opStatus = post(request, response);
  else
    opStatus = Not_Implemented;

  return (opStatus);
}

HttpResponse *Server::process(std::string &buffer) {
  HttpRequest *request = HttpRequestFactory::createFrom(buffer, locations);
  HttpResponse *response = new HttpResponse();
  HttpStatusCode status = HttpRequestFactory::check(request);

  if (status == Ready) {
    response->setProtocol("HTTP", request->getProtocolMainVersion(),
                                  request->getProtocolSubVersion());
    status = resolve(request, response);
  }

  if (status != Ready) {
    HttpResponseComposer::buildErrorResponse(response, status, error_pages, \
                                          request->getProtocolMainVersion(), \
                                          request->getProtocolSubVersion());
  }

  delete request;
  return response;
}

HttpStatusCode Server::post(HttpRequest *request, HttpResponse *response) {
  HttpStatusCode opStatus = Ready;
  response->setLastModifiedTime(
      HttpTime::getLastModifiedTime(request->getResource()));

  // BODY SIZE CHECK
  //  MESMA LOGICA DE CHECAGEM COM STATUS CODE EM REQUEST, VAI FICAR ASSIM ?
  if (request->getRedirectionCode() != 0) {
    response->setStatusCode(request->getRedirectionCode());
    response->setLocation(request->getRedirectionPath());
    return (Ready);
  }

  if (srv_max_body_size != SIZE_T_MAX) {
    if (request->getContentLength() > srv_max_body_size) {
      return (Payload_Too_Large);
    }
  }
  // END BODY SIZE CHECK ---

  PostType pType = request->getPostType();
  
  // WE NEED TO CHANGE THIS TO A SWITCH AND ENCAPSULATE THE LOGIC

  if (pType == None) {
    throw std::runtime_error("Wrong POST REQUEST, NONE");
  } else if (pType == Chunked) {
    std::string location = request->getLocationWithoutIndex();

    if (FileSystem::check(location, F_OK) != 0) {
      response->setStatusCode(No_Content);
    } else {
      response->setStatusCode(Created);
    }
    
    // wrap this inside a proper class 
    std::ofstream file(location.c_str(),
                       std::ofstream::out | std::ofstream::trunc);

    if (file.is_open()) {
      file << request->getRequestBody();
      file.close();
    } else {
      opStatus = Internal_Server_Error;
    }
    
  } else if (pType == Multipart) {
    MultiPartMap multiParts = request->getMultipartMap();
    MultiPartMap::iterator it = multiParts.begin();
    MultiPartMap::iterator ite = multiParts.begin();
    
    // CREATE FILES AND INSERT CONTENT INSIDE;
    for (; it != ite; it++) {
      std::string location = request->getLocationWithoutIndex();
      std::string filename = location + '/' +  it->first + "_fromClient";
      std::cout << filename << std::endl;

      if(FileSystem::check(filename, F_OK)) {
        response->setStatusCode(No_Content);
      }
      else {
        response->setStatusCode(Created);
      }

      // wrap this inside a proper class 
      std::ofstream file(filename.c_str(),
                         std::ofstream::out | std::ofstream::trunc);

      if (file.is_open()) {
        file << it->second;
        file.close();
      } else {
        opStatus = Internal_Server_Error;
      }
    }
  } else if (pType == UrlEncoded) {
      std::string location = request->getLocationWithoutIndex() + "_URL";

      if (FileSystem::check(location, F_OK) == Ready) {
        response->setStatusCode(204);
      } else {
        response->setStatusCode(201);
      }

      std::ofstream file(location.c_str(),
                         std::ofstream::out | std::ofstream::trunc);

      if (file.is_open()) {
        file << request->getRequestBody();
        file.close();
      } else {
        opStatus = Internal_Server_Error;
      }
  }

  if (opStatus != 0) {
    return opStatus;
  }

  response->setContentType(MimeType::identify(request->getResource()));
  response->setContentLength(0);
  return (Ready);
}

HttpStatusCode Server::get(HttpRequest *request, HttpResponse *response) {
  if (request->getRedirectionCode() != 0) {
    response->setStatusCode(request->getRedirectionCode());
    response->setLocation(request->getRedirectionPath());
    return (Ready);
  }

  std::string fullpath = request->getRoot() + request->getResource();
  HttpStatusCode opStatus = Ready;

  if (FileSystem::isDirectory(fullpath)) {
    opStatus = FileSystem::check(fullpath, R_OK | X_OK);

    if (opStatus != 0 || !request->isDirListActive()) {
      return (Forbidden);
    }
    
    if (FileSystem::check(request->getIndexPath(), F_OK) != 0 && \
        request->isDirListActive()) {
      std::map<std::string, struct file_info*> entries;
      opStatus = FileReader::getDirContent(fullpath.c_str(), entries); 

      if (opStatus != Ready) {
        return (opStatus);
      }

      HttpResponseComposer::buildDirListResponse(request, response, entries);
      return (Ready); // or opStatus Code
    } else {
      fullpath = request->getIndexPath();
    }
  } 

  opStatus = FileSystem::check(fullpath, F_OK | R_OK);

  if (opStatus != 0) {
    return (opStatus);
  }

  std::string unmodifiedTimestmap = request->getModifiedTimestampCheck();

  if (!HttpTime::isModifiedSince(unmodifiedTimestmap, fullpath)) {
    response->setStatusCode(Not_Modified);
    return (Ready);
  }
  
  char *resourceData;
  long long resourceSize;
  opStatus = FileReader::getContent(fullpath.c_str(), \
                                    &resourceData, &resourceSize);

  if (opStatus != 0) {
    return (opStatus);
  }

  response->setLastModifiedTime(HttpTime::getLastModifiedTime(fullpath));
  response->setContentType(MimeType::identify(fullpath));
  response->setMsgBody(resourceData);
  response->setContentLength(resourceSize);
  return (Ready);
}

HttpStatusCode Server::del(HttpRequest *request, HttpResponse *response) {
  std::ifstream inputFile;
  HttpStatusCode opStatus = Ready;

  // we need to rework this. a file can only be deleted if:
  // the folder it is contained has EXECUTE permission
  // the file itself has write permission
  if (access(request->getResource().c_str(), F_OK) == -1) {
    opStatus = Not_Found;
  } else if (access(request->getResource().c_str(), R_OK | W_OK) == -1) {
    opStatus = Method_Not_Allowed;
  } else if (remove(request->getResource().c_str()) != 0) {
    opStatus = Internal_Server_Error;
  }

  if (opStatus != Ready) {
    return (opStatus);
  }

  std::string msg = "File succefully deleted\n";
  char *msgBody = new char[msg.size()];

  for (size_t i = 0; i < msg.size(); i++) {
    msgBody[i] = msg[i];
  }

  response->setStatusCode(No_Content);
  response->setMsgBody(msgBody);
  response->setContentLength(msg.size());
  return (Ready);
}

int   Server::getPort(void) {
  return (port);
}
