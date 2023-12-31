/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:22:33 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/10 07:12:39 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Server.hpp"

#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <algorithm>
#include <fstream>
#include <iostream>  // cout
#include <sstream>   // stringstream
#include <string>

#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/HttpResponseComposer.hpp"
#include "../../include/http/HttpStatus.hpp"
#include "../../include/http/HttpTime.hpp"
#include "../../include/http/MimeType.hpp"
#include "../../include/io/FileReader.hpp"
#include "../../include/io/FileSystem.hpp"
#include "../../include/io/FileWriter.hpp"
#include "../../include/io/TcpServerSocket.hpp"

Server::Server(const struct s_serverConfig& config) {
  for (size_t i = 0; i < config.ports.size(); i++) {
    ports.push_back(strtol(config.ports[i].c_str(), NULL, 0));
  }
  
  host = config.host;
  //server_name = config.server_name;
  server_names = config.server_names;
  srv_max_body_size = config.srv_max_body_size;
  error_pages = config.error_page;
  locations = config.location;
}

Server::~Server(void) {}

void Server::setControllerPtr(Controller *controllerPtr) {
  this->controllerPtr = controllerPtr;
}

void setNonBlocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    perror("fcntl");
    exit(EXIT_FAILURE);
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    perror("fcntl");
    exit(EXIT_FAILURE);
  }
}

HttpStatusCode Server::getCGI(Client *client, HttpRequest *request) {
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  std::stringstream ss(request->getPort());
  int porta;
  ss >> porta;
  controllerPtr->addCGItoEpoll(pipefd[0], porta, client);
  pid_t childPid = fork();
  if (childPid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if (childPid == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    std::string path = request->getCGIPath();
    char *argv[] = {const_cast<char *>("php-cgi"),
                    const_cast<char *>(path.c_str()), NULL};
    char **env = createCGIEnv(request);
    close(pipefd[1]);
    execve(path.c_str(), argv, env);
    perror("execve");
    exit(EXIT_FAILURE);
  } else {
    close(pipefd[1]);
    return (CGI);
  }
  return (Ready);
}

HttpStatusCode Server::resolve(Client *client, HttpRequest *request,
                               HttpResponse *response) {
  std::string uTimestamp = request->getUnmodifiedSinceTimestamp();
  if (!uTimestamp.empty() &&
      HttpTime::isModifiedSince(uTimestamp, request->getResource())) {
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
    opStatus = get(client, request, response);
  else if (requestMethod == "DELETE")
    opStatus = del(request, response);
  else if (requestMethod == "POST")
    opStatus = post(client, request, response);
  else
    opStatus = Not_Implemented;

  return (opStatus);
}

HttpStatusCode Server::process(Client *client, HttpRequest *req,
                               HttpResponse *res) {
  std::string buffer = client->getBuffer();
  HttpRequestFactory::setupRequest(req, buffer, locations);
  HttpStatusCode status = HttpRequestFactory::check(req, server_name);

  if (status == Ready) {
    res->setProtocol("HTTP", req->getProtocolMainVersion(),
                     req->getProtocolSubVersion());
    status = resolve(client, req, res);
  }
  if (status == CGI) {
    return status;
  }
  if (status != Ready) {
    HttpResponseComposer::buildErrorResponse(res, status, error_pages,
                                             req->getProtocolMainVersion(),
                                             req->getProtocolSubVersion());
  }
  return Ready;
}

char **Server::createCGIEnv(HttpRequest *request) {
  char **env;
  char *buf;
  int i = 0;
  std::map<std::string, std::string> env_tmp;
  std::stringstream ss;
  std::string tmp2;
  env_tmp.insert(std::pair<std::string, std::string>("PATH_INFO", "/"));
  env_tmp.insert(std::pair<std::string, std::string>(
      "PATH_TRANSLATED", request->getAbsolutePath()));
  env_tmp.insert(
      std::pair<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
  env_tmp.insert(std::pair<std::string, std::string>("REQUEST_METHOD",
                                                     request->getMethod()));
  env_tmp.insert(std::pair<std::string, std::string>(
      "CONTENT_TYPE", request->getContentType()));
  if (request->getMethod() == "GET") {
    env_tmp.insert(std::pair<std::string, std::string>("CONTENT_LENGTH", "0"));
  }
  if (request->getMethod() == "POST") {
    ss << request->getBodyNotParsed().size() - 1;
    env_tmp.insert(
        std::pair<std::string, std::string>("CONTENT_LENGTH", ss.str()));
  }
  env_tmp.insert(std::pair<std::string, std::string>(
      "QUERY_STRING", request->getQueryString()));
  env_tmp.insert(std::pair<std::string, std::string>("SERVER_NAME",
                                                     request->getServerName()));
  env_tmp.insert(
      std::pair<std::string, std::string>("SERVER_PORT", request->getPort()));
  env_tmp.insert(
      std::pair<std::string, std::string>("SERVER_PROTOCOL", "HTTP/1.1"));
  env_tmp.insert(
      std::pair<std::string, std::string>("SERVER_SOFTWARE", "nginxcelent"));
  env_tmp.insert(std::pair<std::string, std::string>("REDIRECT_STATUS", "200"));

  env = new char *[env_tmp.size() + 1];
  std::map<std::string, std::string>::iterator it;
  std::map<std::string, std::string>::iterator its;
  std::map<std::string, std::string>::iterator ite;
  its = env_tmp.begin();
  ite = env_tmp.end();
  for (it = its; it != ite; it++) {
    tmp2 = it->first + "=" + it->second;
    buf = new char[tmp2.size() + 1];
    strncpy(buf, tmp2.c_str(), tmp2.size() + 1);
    env[i] = buf;
    i++;
  }
  env[i] = NULL;
  return env;
}

HttpStatusCode Server::postCGI(Client *client, HttpRequest *request) {
  std::string cgiPath = request->getLocationWithoutIndex();
  int pipe_to_child[2];
  int pipe_to_parent[2];
  if (pipe(pipe_to_child) == -1 || pipe(pipe_to_parent) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
    std::string towrite = request->getBodyNotParsed();
  std::stringstream ss(request->getPort());
  int porta;
  ss >> porta;
  controllerPtr->addCGItoEpoll(pipe_to_parent[0], porta, client);
  pid_t childPid = fork();

  if (childPid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if (childPid == 0) {
    dup2(pipe_to_child[0], STDIN_FILENO);
    dup2(pipe_to_parent[1], STDOUT_FILENO);
    close(pipe_to_child[1]);
    close(pipe_to_parent[0]);

    char **env = createCGIEnv(request);
    std::string path = request->getCGIPath();
    char *argv[] = {const_cast<char *>("php-cgi"),
                  const_cast<char *>(path.c_str()), NULL};
    execve(request->getCGIPath().c_str(), argv, env);
    perror("execve");
    exit(EXIT_FAILURE);
  } else {
    close(pipe_to_child[0]);
    close(pipe_to_parent[1]);
    write(pipe_to_child[1], towrite.c_str(), towrite.size());
    close(pipe_to_child[1]);
    int status;
    waitpid(childPid, &status, WNOHANG);
  }
  return (CGI);
}

HttpStatusCode Server::post(Client *client, HttpRequest *request,
                            HttpResponse *response) {
  HttpStatusCode opStatus = Ready;
  response->setLastModifiedTime(
      HttpTime::getLastModifiedTime(request->getResource()));
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

  PostType pType = request->getPostType();

  if (pType == None) {
    throw std::runtime_error("Wrong POST REQUEST, NONE");
  }

  if (request->getCGI()) {
    std::string location = request->getLocationWithoutIndex();
    std::string ext = request->getCGIExtension();
    std::string checkExt = location.substr(location.size() - ext.size());
    if (checkExt == ext) {
      opStatus = postCGI(client, request);
      return opStatus;
    }
  } else {
    if (pType == Chunked || pType == UrlEncoded) {
      std::string location = request->getLocationWithoutIndex();
      if (FileSystem::check(location, F_OK) != 0) {
        response->setStatusCode(No_Content);
      } else {
        response->setStatusCode(Created);
      }

      opStatus = FileWriter::writeToFile(location, request->getRequestBody());
    } else if (pType == Multipart) {
      MultiPartMap multiParts = request->getMultipartMap();
      MultiPartMap::iterator it = multiParts.begin();
      MultiPartMap::iterator ite = multiParts.end();

      for (; it != ite; it++) {
        std::string location = request->getLocationWithoutIndex();
        std::string filename = location + '/' + it->first + "_fromClient";
        std::cout << filename << std::endl;

        if (FileSystem::check(filename, F_OK) != Ready) {
          response->setStatusCode(Created);
        } else {
          response->setStatusCode(No_Content);
        }

        opStatus = FileWriter::writeToFile(filename, it->second);
      }
    }
  }

  if (opStatus != Ready && opStatus != No_Content && opStatus != Created) {
    return opStatus;
  }
  response->setContentType(MimeType::identify(request->getResource()));
  response->setContentLength(0);
  return (Ready);
}

HttpStatusCode Server::get(Client *client, HttpRequest *request,
                           HttpResponse *response) {
  if (request->getRedirectionCode() != 0) {
    response->setStatusCode(request->getRedirectionCode());
  }

  std::string fullpath = request->getIndexPath();
  HttpStatusCode opStatus = Ready;

  if (FileSystem::isDirectory(fullpath)) {
    opStatus = FileSystem::check(fullpath, R_OK | X_OK);

    if (opStatus != 0 || !request->isDirListActive()) {
      return (Not_Found);
    }

    if (FileSystem::check(request->getIndexPath(), F_OK) == 0 &&
        request->isDirListActive()) {
      std::map<std::string, struct file_info *> entries;
      opStatus = FileReader::getDirContent(fullpath.c_str(), entries);

      if (opStatus != Ready) {
        return (opStatus);
      }

      HttpResponseComposer::buildDirListResponse(request, response, entries);
      return (Ready);
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
  if (request->getCGI()) {
    std::string location = request->getLocationWithoutIndex();
    std::string ext = request->getCGIExtension();
    std::string checkExt = location.substr(location.size() - ext.size());
    if (checkExt == request->getCGIExtension()) {
      opStatus = getCGI(client, request);
      return opStatus;
    }
  }
  char *resourceData;
  long long resourceSize;
  opStatus =
      FileReader::getContent(fullpath.c_str(), &resourceData, &resourceSize);

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
  std::string path = request->getIndexPath();
  if (access(path.c_str(), F_OK) == -1) {
    opStatus = Not_Found;
  } else if (access(path.c_str(), R_OK | W_OK) == -1) {
    opStatus = Method_Not_Allowed;
  } else if (remove(path.c_str()) != 0) {
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

int Server::getPort(void) { return (port); }

std::string Server::getHost(void) { return (host); }

std::vector<std::string> &Server::getServerNames(void) {
  return (server_names);
}

std::vector<size_t> Server::getPorts(void) {
  return (this->ports);
}


