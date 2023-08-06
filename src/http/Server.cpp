/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:22:33 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/05 21:32:40 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Server.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/HttpResponseComposer.hpp"
#include "../../include/http/MimeType.hpp"
#include "../../include/socket/TcpServerSocket.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <sstream>  // stringstream

Server::Server(unsigned int nPort) : port(nPort), connection_fd(-1) {
  socket = new TCPServerSocket(nPort);
}

Server::~Server(void) {
  delete socket;
}

void  Server::start(void) {
  socket->bindAndListen();
  std::cout << "Server is listening on port: " << port << " ..." << std::endl;

  socket->handleConnections(this);
}

void  Server::resolve(HttpRequest *request, HttpResponse *response) {
  // check the protocol version
  std::string requestMethod = request->getMethod();

  if (requestMethod == "GET")
    get(request, response);
  else if (requestMethod == "HEAD")
    head(request, response);
  else
    HttpResponseComposer::buildErrorResponse(response, 501, \
                       request->getProtocolMainVersion(), \
                       request->getProtocolSubVersion());
}

std::string Server::process(char *buffer) {
  HttpRequest *request = HttpRequestFactory::createFrom(buffer);
  HttpResponse response;

  int status = HttpRequestFactory::check(request);
  if (status != 0) {
    HttpResponseComposer::buildErrorResponse(&response, \
                                          status, \
                                          request->getProtocolMainVersion(), \
                                          request->getProtocolSubVersion());
  } else {
    resolve(request, &response);
  }

  delete request;

  return response.getHeaders();
}

void Server::get(HttpRequest *request, HttpResponse *response) {
  std::ifstream inputFile;
  int protoMain = request->getProtocolMainVersion();
  int protoSub = request->getProtocolSubVersion();

  if (access(request->getResource().c_str(), F_OK) == -1) {
    HttpResponseComposer::buildErrorResponse(response, 404, protoMain, protoSub);
    return;
  }

  if (access(request->getResource().c_str(), R_OK) == -1) {
    HttpResponseComposer::buildErrorResponse(response, 403, protoMain, protoSub);
    return;
  }

  inputFile.open(request->getResource().c_str(), std::ios::binary);
  if (!inputFile.is_open()) {
    std::cout << "Resource not found\n";
    return;
  }

  std::vector<char> resourceData;
  char byte = 0;
  while (inputFile.get(byte)) {
    resourceData.push_back(byte);
  }

  inputFile.close();

  response->setProtocol("HTTP", protoMain, protoSub);
  response->setContentType(MimeType::identify(request->getResource()));
  response->setMsgBody(resourceData);
  response->setContentLength(resourceData.size());
}

void Server::head(HttpRequest *request, HttpResponse *response) {
  this->get(request, response);
  std::vector<char> empty;
  response->setMsgBody(empty);
}


