/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:22:33 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/29 18:09:15 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Server.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/MimeType.hpp"
#include "../../inc/socketClass.hpp"

#include <iostream>
#include <fstream>
#include <vector>

Server::Server(void) {
  port = 8080;
  connection_fd = -1;
}

Server::~Server(void) {}

void  Server::resolve(HttpRequest *request, HttpResponse *response) {
  if (request->getMethod() == "GET")
    get(request, response);
  else
    std::cout << "Op not supported yet" << std::endl;
}

void  Server::startupAndListen(void) {

  socket.bindAndListen(port);
  std::cout << "Server is listening on port: " << port << " ..." << std::endl;

  connection_fd = socket.acceptConnection();
  std::cout << "Connection established. Waiting for data..." << std::endl;

}

void Server::handleConnections(void) {
  char buffer[1024];
  bool connected = true;

  while (connected) {
    int bytesRead = socket.receiveData(connection_fd, buffer, 1024);
    buffer[bytesRead] = '\0';

    if (bytesRead != 0) {
      std::cout << "Received data: " << buffer;
      HttpRequest *request = HttpRequestFactory::createFrom(buffer);
      HttpResponse response;
      resolve(request, &response);
      std::string responseMsg = response.getHeaders();
      socket.sendData(connection_fd, responseMsg.c_str(), responseMsg.size());
      // cleanup
      memset(buffer, 0, bytesRead);
      delete request;
    }
  }
  socket.closeConnection(connection_fd);
}

void Server::get(HttpRequest *request, HttpResponse *response) {
  std::ifstream inputFile;

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
}
