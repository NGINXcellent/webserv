/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:22:33 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/28 23:08:38 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Server.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../inc/socketClass.hpp"

#include <iostream>
#include <fstream>
#include <vector>

void  Server::execute(HttpRequest *request, HttpResponse *response) {
  // for the demo request will be a dummy object
  std::ifstream inputFile;

  inputFile.open(request->getResource().c_str(), std::ios::binary);
  if (!inputFile.is_open()) {
    std::cerr << "Resource not found\n";
  }

  std::vector<char> resourceData;
  char byte = 0;
  while (inputFile.get(byte)) {
    resourceData.push_back(byte);
  }

  inputFile.close();
  response->setMsgBody(resourceData);
}

void  Server::startupAndListen(void) {
  TCPServerSocket serverSocket;

  int port = 8080;
  serverSocket.bindAndListen(port);
  std::cout << "Server is listening on port: " << port << " ..." << std::endl;

  int connection = serverSocket.acceptConnection();
  std::cout << "Connection established. Waiting for data..." << std::endl;

  char buffer[100];
  bool connected = true;
  while (connected) {
    int bytesRead = serverSocket.receiveData(connection, buffer, 100);
    buffer[bytesRead] = '\0';

    if (bytesRead != 0) {
      std::cout << "Received data: " << buffer;
      HttpRequest *request = HttpRequestFactory::createFrom(buffer);
      HttpResponse response;
      Server::execute(request, &response);
      std::string responseMsg = response.getHeaders();
      std::cout << "\n ================= RESPONSE =============" << std::endl;
      std::cout << responseMsg << std::endl;
      serverSocket.sendData(connection, responseMsg.c_str(), responseMsg.size());

      // cleanup
      memset(buffer, 0, sizeof(buffer));
      delete request;
    }
  }
  //  close conection and end program
  serverSocket.closeConnection(connection);
}
