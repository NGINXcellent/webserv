/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServerSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:40:52 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/30 21:10:19 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/socket/TcpServerSocket.hpp"
#include "../../include/http/Server.hpp"

// Constructor Server Socket
TCPServerSocket::TCPServerSocket(unsigned int hPort) : sockfd(-1), \
                                              epollfd(-1), \
                                              hostPort(hPort) {}

int TCPServerSocket::bindAndListen() {
  // 			Create epollfd
  epollfd = epoll_create1(0);
  if (epollfd == -1) {
    std::cerr << "Failed to create epoll. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Configurar o endereço para o socket
  sockaddr_in serverAddr;
  bzero(&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(hostPort);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cerr << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    std::cerr << "Failed to bind to port " << hostPort<< ". errno: " << errno
              << std::endl;
    exit(EXIT_FAILURE);
  }

  // Colocar o socket em modo de escuta
  if (listen(sockfd, 10) < 0) {
    std::cerr << "Failed to listen on socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Bind sockfd on epoll event
  events->events = EPOLLIN;
  events->data.fd = sockfd;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, events) == -1) {
    std::cerr << "Failed to add sockfd to epoll. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  return sockfd;
}

void TCPServerSocket::addNewConnection() {
  int newConnection;
  struct sockaddr_in clientToAdd;
  socklen_t len = sizeof(clientToAdd);

  newConnection = accept(sockfd, (struct sockaddr *)&clientToAdd, &len);
  if (newConnection < 0) {
    std::cerr << "Failed to grab new connection. errno: " << errno << std::endl;
  } else {
    // try to add to epoll
    // // EPOLLOUT is not implemented yet
    events->events = EPOLLIN | EPOLLRDHUP | EPOLLOUT;
    events->data.fd = newConnection;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newConnection, events) == -1) {
      std::cerr << "Failed to add new connection to epoll. errno: " << errno
                << std::endl;
      close(newConnection);
    } else {
      // add this conection to conections vector
      connections.push_back(newConnection);
    }
  }
}

void TCPServerSocket::handleConnections(Server *server) {
  // create just one pool of events, its like a line, if is a new conection, add
  // to poll, if not, handle the client conection.
  while (true) {
    int numEvents = epoll_wait(epollfd, events, MAX_EVENTS, 0);
    if (numEvents == -1) {
      std::cerr << "epoll_wait error. errno: " << errno << std::endl;
      continue;
    }
    for (int i = 0; i < numEvents; ++i) {
      int currentFd = events[i].data.fd;    // this client fd
      int currentEvent = events[i].events;  // this client event state
      // in the future, we will iterate thru servers(sockfd) so the nextline
      // will be a funcion.
      if (currentFd == sockfd) {
        // found new conection, try to add connection
        addNewConnection();
        std::cout << "new conection" << std::endl;
      } else if ((currentEvent & EPOLLRDHUP) == EPOLLRDHUP) {
        epoll_ctl(epollfd, EPOLL_CTL_DEL, currentFd, NULL);
        std::cout << "Connection with FD -> " << currentFd
                  << " is closed by client" << std::endl;
        for (std::vector<int>::iterator it = connections.begin();
             it != connections.end(); ++it) {
          if (*it == currentFd) {
            connections.erase(it);
            break;
          }
        }
      } else if ((currentEvent & EPOLLIN) == EPOLLIN) {
        // If connection already exist
        char buffer[1024];
        bzero(buffer, sizeof(buffer));
        int bytesRead = read(currentFd, buffer, sizeof(buffer));
        if (bytesRead <= 0) {
          // Connection error or close, we remove from epoll
          // signal(SIGINT, handleSIGINT); localtion is here?
          epoll_ctl(epollfd, EPOLL_CTL_DEL, currentFd, NULL);
          close(currentFd);
          for (std::vector<int>::iterator it = connections.begin();
               it != connections.end(); ++it) {
            if (*it == currentFd) {
              connections.erase(it);
              break;
            }
          }
        } else {
          // the idea here is to have an request handler to deal with requests
          // std::cout << "Received data from fd " << currentFd << ": " << buffer;
          // std::string response = "Good talking to you fd\n";
          // sendData(currentFd, response.c_str(), response.size());
          std::string response = server->process(buffer);
          sendData(currentFd, response.c_str(), response.size());
        }
      }
      // // EPOLLOUT means this FD is ready to write, so, is like while true
      // full. else if ((currentEvent & EPOLLOUT) == EPOLLOUT) {
      //         std::string response = "Good talking to you fd\n";
      //         sendData(currentFd, response.c_str(), response.size());
      // }
    }
  }
}

int TCPServerSocket::receiveData(int connection, char *buffer, int bufferSize) {
  int bytesRead = read(connection, buffer, bufferSize);
  return bytesRead;
}

// send data to conection fd
void TCPServerSocket::sendData(int connection, const char *data, int dataSize) {
  send(connection, data, dataSize, 0);
}

// close conection fd
void TCPServerSocket::closeConnection(int connection) { close(connection); }

// simple destructor
TCPServerSocket::~TCPServerSocket() {
  if (epollfd != -1) {
    close(epollfd);
  }
  if (sockfd != -1) {
    close(sockfd);
  }
}