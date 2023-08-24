/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:22:33 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/24 17:45:11 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Server.hpp"
#include "../../include/http/HttpTime.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/HttpResponseComposer.hpp"
#include "../../include/http/MimeType.hpp"
#include "../../include/socket/TcpServerSocket.hpp"
#include "../../include/utils/FileReader.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>  // stringstream
#include <sys/stat.h>
#include <algorithm>

Server::Server(const struct s_serverConfig& config) {
  port = strtol(config.port.c_str(), NULL, 0);
  host = config.host;
  server_name = config.server_name;
  max_body_size = strtol(config.port.c_str(), NULL, 0);
  error_pages = config.error_page;
  locations = config.location;
}

Server::~Server(void) {
  delete socket;
}

int Server::resolve(HttpRequest *request, HttpResponse *response) {
  std::string uTimestamp = request->getUnmodifiedSinceTimestamp();

  if (!uTimestamp.empty() && \
      HttpTime::isModifiedSince(uTimestamp, request->getResource())) {
    // shim
    return 412;
  }

  std::string requestMethod = request->getMethod();
  std::vector<std::string> allowedMethods = request->getAllowedMethods();
  std::vector<std::string>::iterator it;
  it = std::find(allowedMethods.begin(), allowedMethods.end(), requestMethod);

  if (!allowedMethods.empty() && it == allowedMethods.end()) {
    return 405;
  }
  
  int opStatus = 0;

  if (requestMethod == "GET")
    opStatus = get(request, response);
  else if (requestMethod == "HEAD")
    opStatus = head(request, response);
  else if (requestMethod == "DELETE")
    opStatus = del(request, response);
  else if (requestMethod == "POST")
    opStatus = post(request, response);
  else
    opStatus = 501; 

 return (opStatus);
}

HttpResponse *Server::process(std::string &buffer) {
  std::cout << "BUFFER" << std::endl;
  std::cout << buffer << std::endl;
  //std::cout << "hey hey >>>>" << buffer.find_first_of('\n') << std::endl;
  HttpRequest *request = HttpRequestFactory::createFrom(buffer, locations);
  HttpResponse *response = new HttpResponse();
  int status = HttpRequestFactory::check(request);

  if (status == 0) {
    status = resolve(request, response);
  }    

  if (status != 0) {
    HttpResponseComposer::buildErrorResponse(response, status, error_pages, \
                                          request->getProtocolMainVersion(), \
                                          request->getProtocolSubVersion());
  }

  delete request;
  return response;
}

bool fileExists(const std::string& filename) {
    return (access(filename.c_str(), F_OK) != -1);
}

int Server::post(HttpRequest *request, HttpResponse *response) {
  int protoMain = request->getProtocolMainVersion();
  int protoSub = request->getProtocolSubVersion();
  int opStatus = 0;
  response->setProtocol("HTTP", protoMain, protoSub);
  response->setLastModifiedTime(
      HttpTime::getLastModifiedTime(request->getResource()));

  if (request->getPostType() == "NONE") {
    throw std::runtime_error("Wrong POST REQUEST, NONE");
  } else if (request->getPostType() == "CHUNK") {
    std::string location = request->getLocationWithoutIndex();

    if (fileExists(location)) {
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
      opStatus = 500;
    }
  } else if (request->getPostType() == "MULTIPART") {
    std::vector<s_multipartStruct> multiParts = request->getMultipartStruct();

    // CREATE FILES AND INSERT CONTENT INSIDE;
    for (size_t i = 1; i < multiParts.size(); i++) {
      std::string location = request->getLocationWithoutIndex();
      std::string filename =
          location + '/' + multiParts[i].name + "_fromClient";
      std::cout << filename << std::endl;

      if(fileExists(filename)) {
        response->setStatusCode(204);
      }
      else {
        response->setStatusCode(201);
      }

      std::ofstream file(filename.c_str(),
                         std::ofstream::out | std::ofstream::trunc);

      if (file.is_open()) {
        file << multiParts[i].content;
        file.close();
      } else {
        opStatus = 500;
      }
    }
  }

  if (opStatus != 0) {
    return opStatus;
  }
  response->setContentType(MimeType::identify(request->getResource()));
  // TODO: SETUP CORRECT CONTENT LENGHT IN POST;
  response->setContentLength(0);
  return (0);
}

int Server::get(HttpRequest *request, HttpResponse *response) {
  int protoMain = request->getProtocolMainVersion();
  int protoSub = request->getProtocolSubVersion();
  response->setProtocol("HTTP", protoMain, protoSub);
  response->setLastModifiedTime(HttpTime::getLastModifiedTime(request->getResource()));

  std::string unmodifiedTimestmap = request->getModifiedTimestampCheck();

  if (!HttpTime::isModifiedSince(unmodifiedTimestmap, request->getResource())) {
    response->setStatusCode(304);
    return (0);
  }

  char *resourceData;
  long long resourceSize;
  int opStatus = FileReader::getContent(request->getResource(), &resourceData, &resourceSize);

  if (opStatus != 0) {
    return (opStatus);
  }
// TEM QUE CHECAR AQUI PRA VER SE VAI FICAR ASSIM.
  if (request->getResponseStatusCode() != 0)
    response->setStatusCode(request->getResponseStatusCode());
  else
    response->setStatusCode(200);

  response->setContentType(MimeType::identify(request->getResource()));
  response->setMsgBody(resourceData);
  response->setContentLength(resourceSize);
  return (0);
}

int Server::head(HttpRequest *request, HttpResponse *response) {
  int protoMain = request->getProtocolMainVersion();
  int protoSub = request->getProtocolSubVersion();
  response->setProtocol("HTTP", protoMain, protoSub);
  int opStatus = 0;

  if (access(request->getResource().c_str(), F_OK) == -1) {
    opStatus = 404;
  } else if (access(request->getResource().c_str(), R_OK) == -1) {
    opStatus = 403;
  }

  if (opStatus != 0) {
    return (opStatus);
  }

  std::string modifiedTimestmap = request->getModifiedTimestampCheck();

  if (!HttpTime::isModifiedSince(modifiedTimestmap, request->getResource())) {
    response->setStatusCode(304);
    return (0);
  }

  response->setLastModifiedTime(HttpTime::getLastModifiedTime(request->getResource()));
  struct stat fileInfo;
  off_t fileSize;

  if (stat(request->getResource().c_str(), &fileInfo) == 0) {
    fileSize = fileInfo.st_size;
  }

  response->setStatusCode(200);
  response->setContentType(MimeType::identify(request->getResource()));
  response->setContentLength(fileSize);
  return (0);
}

int Server::del(HttpRequest *request, HttpResponse *response) {
  std::ifstream inputFile;
  int protoMain = request->getProtocolMainVersion();
  int protoSub = request->getProtocolSubVersion();
  int opStatus = 0;

  if (access(request->getResource().c_str(), F_OK) == -1) {
    opStatus = 404;
  } else if (access(request->getResource().c_str(), R_OK | W_OK) == -1) {
    opStatus = 405;
  } else if (remove(request->getResource().c_str()) != 0) {
    opStatus = 500;
  }

  if (opStatus != 0) {
    return (opStatus);
  }

  std::string msg = "File succefully deleted\n";
  char *msgBody = new char[msg.size()];

  for (size_t i = 0; i < msg.size(); i++) {
    msgBody[i] = msg[i];
  }

  response->setStatusCode(204);
  response->setMsgBody(msgBody);
  response->setProtocol("HTTP", protoMain, protoSub);
  response->setContentLength(msg.size());
  return (0);
}

int   Server::getPort(void) {
  return (port);
}
