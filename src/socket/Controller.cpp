/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Controller.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:51:31 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/18 16:49:29 by lfarias-         ###   ########.fr       */
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
    this->serverPool.insert(std::make_pair(newServer->getPort(), newServer));
    ++it;
  }
}

void Controller::endServer() {
  exit(0);
}

void Controller::signalHandler(int signal) {
  if (signal == SIGINT) {
    std::cout << "\nServer is stopping gracefully, goodbye !" << std::endl;
    endServer();
  }
}

Controller::~Controller(void) {
  // todo: Implement resource liberation logic
}

void Controller::init(void) {
  //  Create epollfd
  std::signal(SIGINT, signalHandler);
  epollfd = epoll_create1(0);
  if (epollfd == -1) {
    std::cerr << "Failed to create epoll. errno: " << errno << std::endl;
      exit(EXIT_FAILURE);
  }

  // start sockets
  std::map<int, Server*>::iterator it = serverPool.begin();
  std::map<int, Server*>::iterator ite = serverPool.end();

  for (; it != ite; ++it) {
    int port = it->second->getPort();;
    TCPServerSocket *socket = new TCPServerSocket(port);
    socket->bindAndListen();
    socketPool.insert(std::make_pair(port, socket));
    std::cout << "[LOG]\tlistening on port: " << socket->getPort() << std::endl;

    // Bind sockfd on epoll event
    events->events = EPOLLIN;
    events->data.fd = socket->getFD();

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket->getFD(), events) == -1) {
      std::cerr << "Failed to add sockfd to epoll. errno: ";
      std::cout << errno << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  handleConnections();
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

      if (isNewConnection(currentFd)) {
        std::cout << "new conection" << std::endl;
        addNewConnection(currentFd);  // if new connection found, add it.
      } else if ((currentEvent & EPOLLRDHUP) == EPOLLRDHUP) {
        std::cout << "Connection with FD -> " << currentFd;
        std::cout << " is closed by client" << std::endl;
        closeConnection(currentFd);
      } else if ((currentEvent & EPOLLIN) == EPOLLIN) {
        readFromClient(currentFd);
      } else if ((currentEvent & EPOLLOUT) == EPOLLOUT) {
        if (bufferPool[currentFd].empty()) {
            i++;
            continue;
        } else {
          sendToClient(currentFd);
        }
      }
    }
    checkTimeOut();
  }
}

void Controller::checkTimeOut() {
  time_t currentTime = time(NULL);
  std::vector<int> clientsToRemove;
  std::map<int, time_t>::iterator it = timeoutPool.begin();

  for (; it != timeoutPool.end(); ++it) {
    if (currentTime - it->second > TIMEOUT) {
      std::cout << "removing client: " << it->first;
      std::cout << " due to timeout" << std::endl;
      clientsToRemove.push_back(it->first);
    }
  }
 for (std::vector<int>::iterator it = clientsToRemove.begin(); it != clientsToRemove.end(); ++it) {
    timeoutList.erase(*it);
    if(closeConnection(*it))
      std::cout << "removing client: " << *it << " due to timeout" << std::endl;
 }
}

int Controller::findConnectionSocket (int socketFD) {
    for (size_t i = 0; i < socketPool.size(); i++) {
    if (socketFD == socketPool[i]->getFD())
      return (i);
  }
  return (-1);
}

void Controller::addNewConnection(int socketFD) {
  int newConnection;
  struct sockaddr_in clientToAdd;
  socklen_t len = sizeof(clientToAdd);
  newConnection = accept(socketFD, (struct sockaddr *)&clientToAdd, &len);

  if (newConnection < 0) {
    std::cerr << "Failed to grab new connection. errno: " << errno << std::endl;
  } else {
    events->events = EPOLLIN | EPOLLRDHUP | EPOLLOUT;
    events->data.fd = newConnection;
    bufferPool[newConnection];  // hacky way to initialize a buffer
    time_t currentTime = time(NULL);
    timeoutPool[newConnection] = currentTime;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newConnection, events) == -1) {
      std::cerr << "Failed to add new connection to epoll. errno: " << errno
                << std::endl;
      close(newConnection);
    } else {
      connections.push_back(newConnection);
    }
  }
}

bool  Controller::closeConnection(int currentFd) {
  epoll_ctl(epollfd, EPOLL_CTL_DEL, currentFd, NULL);
  close(currentFd);
  std::vector<int>::iterator it = connections.begin();

  for (; it != connections.end(); ++it) {
    if (*it == currentFd) {
      connections.erase(it);
      return true;
    }
  }
  return false;
}

bool Controller::isNewConnection(int currentFD) {
  std::map<int, TCPServerSocket*>::iterator it = socketPool.begin();
  std::map<int, TCPServerSocket*>::iterator ite = socketPool.end();

  for (; it != ite; ++it) {
    if (currentFD == it->second->getFD())
      return (true);
  }

  return (false);
}

void Controller::readFromClient(int currentFd) {
  bzero(buffer, 1024);
  int bytesRead = read(currentFd, buffer, 1024);

  if (bytesRead > 0) {
    for (int i = 0; i < bytesRead; i++) {
        bufferPool[currentFd].push_back(buffer[i]);
    }
    return;
  }

  // Connection error or close, we remove from epoll
  std::cout << "Connection with FD -> " << currentFd;
  std::cout << " is closed by server" << std::endl;
  closeConnection(currentFd);
}

void Controller::sendToClient(int currentFd) {
  struct sockaddr_in address;
  socklen_t sockAddrLen = sizeof(address);

  if (getsockname(currentFd, (struct sockaddr *)&address, &sockAddrLen) < 0) {
    std::cout << "Error trying to get socket name" << std::endl;
    exit(1);
  }

  bufferPool[currentFd].push_back('\0');
  int port = ntohs(address.sin_port);
  Server *server = serverPool[port];
  std::string response = server->process(bufferPool[currentFd]);
  TCPServerSocket *socket = socketPool[port];
  socket->sendData(currentFd, response.c_str(), response.size());
  std::vector<char>().swap(bufferPool[currentFd]);  // release memory and clean buffer
}

