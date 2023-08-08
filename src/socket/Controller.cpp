/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Controller.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:51:31 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/08 15:56:25 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/socket/Controller.hpp"
#include "../../include/socket/TcpServerSocket.hpp"
#include <sys/socket.h>

Controller::Controller(const InputHandler &input) {
  std::vector<struct s_serverConfig>::iterator it = input.serverVector->begin();
  std::vector<struct s_serverConfig>::iterator end = input.serverVector->end();

  while (it != end) {
    const struct s_serverConfig &serverConfig = *it;
    Server *newServer = new Server(serverConfig);
    this->serverList.push_back(newServer);
    ++it;
  }

  buffer = new char[1024];
  bzero(buffer, 1024);
}

Controller::~Controller(void) {}

void Controller::init(void) {
  //  Create epollfd

  epollfd = epoll_create1(0);
  if (epollfd == -1) {
    std::cerr << "Failed to create epoll. errno: " << errno << std::endl;
      exit(EXIT_FAILURE);
  }

  // start sockets
  for (size_t i = 0; i < serverList.size(); i++) {
    TCPServerSocket *socket = new TCPServerSocket(serverList[i]->getPort());
    socket->bindAndListen();
    socketList.push_back(socket);
    // std::cout << "Server is listening on port: " << port << " ..." << std::endl;

    // Bind sockfd on epoll event
    events->events = EPOLLIN;
    events->data.fd = socket->getFD();

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket->getFD(), events) == -1) {
      std::cerr << "Failed to add sockfd to epoll. errno: ";
      std::cout << errno << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

void Controller::addNewConnection(int socketFD) {
  int newConnection;
  struct sockaddr_in clientToAdd;
  socklen_t len = sizeof(clientToAdd);
  newConnection = accept(socketFD, (struct sockaddr *)&clientToAdd, &len);

  if (newConnection < 0) {
    std::cerr << "Failed to grab new connection. errno: " << errno << std::endl;
  } else {
    // // EPOLLOUT is not implemented yet
    events->events = EPOLLIN | EPOLLRDHUP | EPOLLOUT;
    events->data.fd = newConnection;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newConnection, events) == -1) {
      std::cerr << "Failed to add new connection to epoll. errno: " << errno
                << std::endl;
      close(newConnection);
    } else {
      connections.push_back(newConnection);
    }
  }
}

void Controller::handleConnections(void) {
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

      if (isSocket(currentFd) != -1) {
        addNewConnection(currentFd);  // if new connection found, add it.
        std::cout << "new conection" << std::endl;
      } else if ((currentEvent & EPOLLRDHUP) == EPOLLRDHUP) {
        epoll_ctl(epollfd, EPOLL_CTL_DEL, currentFd, NULL);
        close(currentFd);
        std::cout << "Connection with FD -> " << currentFd;
        std::cout << " is closed by client" << std::endl;
        std::vector<int>::iterator it = connections.begin();

        for (; it != connections.end(); ++it) {
          if (*it == currentFd) {
            connections.erase(it);
            break;
          }
        }
      }
      else if ((currentEvent & EPOLLIN) == EPOLLIN) {
        // If connection already exist

        //bufferList.insert(std::make_pair(currentFd, buffer));
        //bzero(bufferList[currentFd], 1024);
        bzero(buffer, 1024);
        int bytesRead = read(currentFd, buffer, 1024);

        if (bytesRead <= 0) {
          // Connection error or close, we remove from epoll
          // signal(SIGINT, handleSIGINT); localtion is here?
          epoll_ctl(epollfd, EPOLL_CTL_DEL, currentFd, NULL);
          close(currentFd);
          std::vector<int>::iterator it = connections.begin();
          std::cout << "Connection with FD -> " << currentFd;
          std::cout << " is closed by server" << std::endl;

          for (; it < connections.end(); ++it) {
            if (*it == currentFd) {
              connections.erase(it);
              break;
            }
          }
        }
        // SERVER PROCESS
      }
      else if ((currentEvent & EPOLLOUT) == EPOLLOUT) {
        // SEND DATA

        if (*buffer == '\0') {
            i++;
            continue;
        }
          struct sockaddr_in address;
          socklen_t sockAddrLen = sizeof(address);

          if (getsockname(currentFd, (struct sockaddr *)&address, &sockAddrLen) < 0) {
            std::cout << "Error trying to get socket name" << std::endl;
            exit(1);
          }

          int port = ntohs(address.sin_port);
          Server *server = NULL;

          for (size_t i = 0; i < serverList.size(); i++) {
            server = serverList[i];
            if (server != NULL && port == server->getPort())
              break;
            else
              server = NULL;
          }
 
          std::cout << "======BUFFER:=======" << std::endl;
          std::cout << buffer << std::endl;
          std::cout << "====================" << std::endl;
          std::string response = server->process(buffer);
          TCPServerSocket *socket = NULL;

          for (size_t i = 0; i < socketList.size(); i++) {
            socket = socketList[i];

            if (static_cast<int>(socket->getPort()) == port)
              break;
            else
              socket = NULL;
          }

          socket->sendData(currentFd, response.c_str(), response.size());
          bzero(buffer, 1024);
        }
    }
  }
}

int Controller::isSocket(int currentFD) {
  for (size_t i = 0; i < socketList.size(); i++) {
    if (currentFD == socketList[i]->getFD())
      return socketList[i]->getFD();
  }
  return (-1);
}
