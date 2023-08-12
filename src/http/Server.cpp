/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:22:33 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/12 19:57:50 by lfarias-         ###   ########.fr       */
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

void  Server::resolve(HttpRequest *request, HttpResponse *response) {
  std::string uTimestamp = request->getUnmodifiedSinceTimestamp();

  if (!uTimestamp.empty() && \
      HttpTime::isModifiedSince(uTimestamp, request->getResource())) {
    // shim
    HttpResponseComposer::buildErrorResponse(response, 412, \
                       error_pages,
                       request->getProtocolMainVersion(), \
                       request->getProtocolSubVersion());
    return;
  }

  std::string requestMethod = request->getMethod();
  std::vector<std::string> allowedMethods = request->getAllowedMethods();
  std::vector<std::string>::iterator it;
  it = std::find(allowedMethods.begin(), allowedMethods.end(), requestMethod);

  if (!allowedMethods.empty() && it == allowedMethods.end()) {
    HttpResponseComposer::buildErrorResponse(response, 405, \
                       error_pages,
                       request->getProtocolMainVersion(), \
                       request->getProtocolSubVersion());
    return;
  }

  if (requestMethod == "GET")
    get(request, response);
  else if (requestMethod == "HEAD")
    head(request, response);
  else if (requestMethod == "DELETE")
    del(request, response);
  else
    HttpResponseComposer::buildErrorResponse(response, 501, \
                       error_pages,
                       request->getProtocolMainVersion(), \
                       request->getProtocolSubVersion());
}

std::string Server::process(char *buffer) {
  HttpRequest *request = HttpRequestFactory::createFrom(buffer, locations);
  HttpResponse response;

  int status = HttpRequestFactory::check(request);

  if (status != 0) {
    HttpResponseComposer::buildErrorResponse(&response, \
                                          status, \
                                          error_pages, \
                                          request->getProtocolMainVersion(), \
                                          request->getProtocolSubVersion());
  } else {
    resolve(request, &response);
  }

  delete request;
  return response.getHeaders();
}

void Server::get(HttpRequest *request, HttpResponse *response) {
  int protoMain = request->getProtocolMainVersion();
  int protoSub = request->getProtocolSubVersion();
  response->setProtocol("HTTP", protoMain, protoSub);
  response->setLastModifiedTime(HttpTime::getLastModifiedTime(request->getResource()));

  std::string unmodifiedTimestmap = request->getModifiedTimestampCheck();

  if (!HttpTime::isModifiedSince(unmodifiedTimestmap, request->getResource())) {
    response->setStatusCode(304);
    return;
  }

  std::vector<char> resourceData;
  int opStatus = FileReader::getContent(request->getResource(), &resourceData);

  if (opStatus != 0) {
    HttpResponseComposer::buildErrorResponse(response, opStatus, error_pages, \
                                             protoMain, protoSub);
    return;
  }

  response->setStatusCode(200);
  response->setContentType(MimeType::identify(request->getResource()));
  response->setMsgBody(resourceData);
  response->setContentLength(resourceData.size());
}

void Server::head(HttpRequest *request, HttpResponse *response) {
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
    HttpResponseComposer::buildErrorResponse(response, opStatus, error_pages, \
                                             request->getProtocolMainVersion(), \
                                             request->getProtocolSubVersion());
    return;
  }

  std::string modifiedTimestmap = request->getModifiedTimestampCheck();

  if (!HttpTime::isModifiedSince(modifiedTimestmap, request->getResource())) {
    response->setStatusCode(304);
    return;
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
}

void Server::del(HttpRequest *request, HttpResponse *response) {
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
    HttpResponseComposer::buildErrorResponse(response, opStatus, error_pages, \
                                             protoMain, protoSub);
    return;
  }

  std::string msgBody = "File succefully deleted\n";

  response->setStatusCode(204);
  response->setMsgBody(std::vector<char>(msgBody.begin(), msgBody.end()));
  response->setProtocol("HTTP", protoMain, protoSub);
  response->setContentLength(msgBody.size());
}

int   Server::getPort(void) {
  return (port);
}

