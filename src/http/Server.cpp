/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:22:33 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/03 17:46:24 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Server.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/MimeType.hpp"
#include "../../include/http/HttpStatus.hpp"
#include "../../include/socket/TcpServerSocket.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <sstream>  // stringstream

int checkRequest(HttpRequest *request);
void buildErrorResponse(HttpResponse *response, int error_code);

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
    buildErrorResponse(response, 501);
}

std::string Server::process(char *buffer) {
  HttpRequest *request = HttpRequestFactory::createFrom(buffer);
  HttpResponse response;

  int status = checkRequest(request);
  if (status != 0) {
    buildErrorResponse(&response, status);
  } else {
    resolve(request, &response);
  }

  delete request;

  return response.getHeaders();
}

void Server::get(HttpRequest *request, HttpResponse *response) {
  std::ifstream inputFile;

  if (access(request->getResource().c_str(), F_OK) == -1) {
    buildErrorResponse(response, 404);
    return;
  }

  if (access(request->getResource().c_str(), R_OK) == -1) {
    buildErrorResponse(response, 403);
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

  response->setContentType(MimeType::identify(request->getResource()));
  response->setMsgBody(resourceData);
  response->setContentLength(resourceData.size());
}

void Server::head(HttpRequest *request, HttpResponse *response) {
  this->get(request, response);
  std::vector<char> empty;
  response->setMsgBody(empty);
}

int checkRequest(HttpRequest *request) {
  if (request->getProtocolVersion() == "HTTP/1.1" && request->getHost().size() == 0)
    return (400);
  return (0);
}

void buildErrorResponse(HttpResponse *response, int error_code) {
  response->setStatusCode(error_code);
  response->setContentType("text/html");

  std::string content;
  content += "<html><head><title>";

  std::stringstream ss;
  ss << error_code;
  std::string errorMsg = HttpStatus::getMessage(error_code);

  content += ss.str() + " " + errorMsg + "</title></head>";
  content += "<body><center><h1>" + ss.str() + " " + errorMsg + \
    "</h1></center></body>";
  content += "<hr><center>webserv/0.1</center></body></html>";

  std::vector<char> responseContent;
  for (size_t i = 0; i < content.size(); i++) {
    responseContent.push_back(content[i]);
  }
  response->setMsgBody(responseContent);
  response->setContentLength(responseContent.size());
}
