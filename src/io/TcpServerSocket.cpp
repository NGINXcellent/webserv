/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServerSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:40:52 by dvargas           #+#    #+#             */
/*   Updated: 2023/11/09 16:30:57 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/TcpServerSocket.hpp"

#include <iostream>

#include "../../include/http/Server.hpp"
#include "../../include/utils/Logger.hpp"

// Constructor Server Socket
TCPServerSocket::TCPServerSocket(unsigned int hPort) : sockfd(-1), \
                                              hostPort(hPort) {}

// simple destructor
TCPServerSocket::~TCPServerSocket() {
  if (sockfd != -1) {
    close(sockfd);
  }
}

int TCPServerSocket::bindAndListen() {
  // Configurar o endereço para o socket
  sockaddr_in serverAddr;
  bzero(&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(hostPort);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    std::cerr << "Failed to create socket. errno: " << errno << std::endl;
    Logger::msg << "Failed to create socket. errno: ";
    Logger::print(Error);
    exit(EXIT_FAILURE);
  }

  if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    Logger::msg << "Failed to bind to port " << hostPort << ". errno: " << errno;
    Logger::print(Error);
    exit(EXIT_FAILURE);
  }

  // Colocar o socket em modo de escuta
  if (listen(sockfd, 10) < 0) {
    Logger::msg << "Failed to listen on socket. errno: " << errno;
    Logger::print(Error);
    exit(EXIT_FAILURE);
   }

  return sockfd;
}

int TCPServerSocket::receiveData(int connection, char *buffer, int bufferSize) {
  int bytesRead = read(connection, buffer, bufferSize);
  return bytesRead;
}

// send data to conection fd
int TCPServerSocket::sendData(int connection, const char *data, int dataSize) {
  return send(connection, data, dataSize, 0);
}

// close conection fd
void TCPServerSocket::closeConnection(int connection) { close(connection); }

int TCPServerSocket::getFD(void) {
  return (sockfd);
}

unsigned int TCPServerSocket::getPort(void) {
  return (hostPort);
}
