/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServerSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:40:52 by dvargas           #+#    #+#             */
/*   Updated: 2023/11/10 05:32:33 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/TcpServerSocket.hpp"

#include <iostream>

#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>

#include "../../include/http/Server.hpp"
#include "../../include/utils/Logger.hpp"

// Constructor Server Socket
TCPServerSocket::TCPServerSocket(unsigned int hPort, const std::string &host) : \
                                sockfd(-1), hostPort(hPort), hostAddr(host) {}

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
  in_addr_t ip_addr = ft_inet_addr(hostAddr.c_str());

  if (ip_addr == INADDR_NONE) {
    Logger::msg << "error while trying to parse host";
    Logger::print(Error);
    return (-1);
  }
    
  serverAddr.sin_addr.s_addr = ip_addr;
  serverAddr.sin_port = htons(hostPort);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    std::cerr << "Failed to create socket. errno: " << errno << std::endl;
    Logger::msg << "Failed to create socket. errno: ";
    Logger::print(Error);
    return (-1);
  }

  if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    Logger::msg << "Failed to bind to port " << hostPort << ". errno: " << errno;
    Logger::print(Error);
    return (-1);
  }

  // Colocar o socket em modo de escuta
  if (listen(sockfd, 10) < 0) {
    Logger::msg << "Failed to listen on socket. errno: " << errno;
    Logger::print(Error);
    return (-1);
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

unsigned long TCPServerSocket::ft_inet_addr(const char *ipAddrString) {
    struct addrinfo hints, *result, *p;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;

    int status = getaddrinfo(ipAddrString, NULL, &hints, &result);

    if (status != 0) {
        return INADDR_NONE;
    }

    unsigned long ipBinary = INADDR_NONE;

    for (p = result; p != NULL; p = p->ai_next) {
      if (p->ai_family == AF_INET) {
          struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
          ipBinary = ipv4->sin_addr.s_addr;
          break; // Use the first IPv4 address found
      }
    }

    freeaddrinfo(result);

    return ipBinary;
}

