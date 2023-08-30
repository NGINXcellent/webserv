/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Controller.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:51:31 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/28 21:31:19 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/Controller.hpp"
#include "../../include/io/TcpServerSocket.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>

int   setToNonBlock(int sock);
void  initEpollEvent(struct epoll_event *ev, uint32_t flag, int fd);
int   getPortFromFd(int connectionFd);

Controller::Controller(const InputHandler &input) {
  std::vector<struct s_serverConfig>::iterator it = input.serverVector->begin();
  std::vector<struct s_serverConfig>::iterator end = input.serverVector->end();

  while (it != end) {
    const struct s_serverConfig &serverConfig = *it;
    Server *newServer = new Server(serverConfig);
    this->serverPool.insert(std::make_pair(newServer->getPort(), newServer));
    ++it;
  }

  events.reserve(100);
}

Controller::~Controller(void) {
  // todo: Implement resource liberation logic
  // implement logic to close the epollfd

  std::map<int, Client*>::iterator clientIt = connectedClients.begin();
  std::map<int, Client*>::iterator clientIte = connectedClients.end();

  for (; clientIt != clientIte; clientIt++) {
    closeConnection(clientIt->first);
  }

  std::map<int, Server*>::iterator serverIt = serverPool.begin();
  std::map<int, Server*>::iterator serverIte = serverPool.end();

  for (; serverIt != serverIte; serverIt++) {
    delete serverIt->second;
  }

  std::map<int, TCPServerSocket*>::iterator socketIt = socketPool.begin();
  std::map<int, TCPServerSocket*>::iterator socketIte = socketPool.end();

  for (; socketIt != socketIte; serverIt++) {
    delete socketIte->second;
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

void Controller::init(void) {
  //  Create epollfd
  std::signal(SIGINT, signalHandler);
  struct epoll_event ev;
  epollfd = epoll_create(1);

  if (epollfd == -1) {
    std::cerr << "Failed to create epoll. errno: " << errno << std::endl;
      exit(EXIT_FAILURE);
  }

  // start sockets
  std::map<int, Server*>::iterator it = serverPool.begin();
  std::map<int, Server*>::iterator ite = serverPool.end();

  for (; it != ite; ++it) {
    int port = it->second->getPort();
    TCPServerSocket *socket = new TCPServerSocket(port);
    socket->bindAndListen();
    socketPool.insert(std::make_pair(port, socket));
    std::cout << "[LOG]\tlistening on port: " << socket->getPort() << std::endl;

    // Bind sockfd on epoll event
    initEpollEvent(&ev, EPOLLIN | EPOLLOUT, socket->getFD());

    if (setToNonBlock(socket->getFD())) {
      throw std::runtime_error("Non Block Error");
    }

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket->getFD(), &ev) == -1) {
      std::cerr << "Failed to add sockfd to epoll. errno: ";
      std::cout << errno << std::endl;
      exit(EXIT_FAILURE);
    } else {
      events.push_back(ev);
    }
  }

  handleConnections();
}

bool isChunkedBodyComplete(const std::string &body) {
  size_t pos = body.find("\r\n0\r\n\r\n", body.size() - 7);
  if (pos != std::string::npos) {
    return true;
  }
  return false;
}

bool isMultipartBodyComplete(const std::string &body) {
  size_t pos = body.find("--\r\n", body.size() - 6);
  if (pos != std::string::npos) {
    return true;
  }
  return false;
}

// we already have an toLowerStr, need to remove this . . .
std::string toLowerStr2(std::string str) {
  std::string result;

  for (size_t i = 0; i < str.size(); i++) {
    result += static_cast<char>(std::tolower(str[i]));
  }

  return (result);
}

bool isHTTPRequestComplete(const std::string &request) {
  size_t pos = request.find("\r\n\r\n");

  if (pos != std::string::npos) {
    size_t contentPos = request.find("\r\n\r\n") + 4;
    std::string body = request.substr(contentPos);

    size_t transferEncodingPos = request.find("Transfer-Encoding: chunked");
    if (transferEncodingPos != std::string::npos) {
      if (isChunkedBodyComplete(body)) {
        return true;  // complete chunk req
      } else {
        return false;  // need more data
      }
    }

    size_t contentTypePos = request.find("Content-Type: ");
    if (contentTypePos != std::string::npos) {
      std::string contentType = request.substr(contentTypePos + 14, 19);
      contentType = toLowerStr2(contentType);
      if (contentType == "multipart/form-data") {
        if (isMultipartBodyComplete(body)) {
          return true;  // complete multipart req
        } else {
          return false;  // need more data
        }
      }
    } else {
      return true;  // complete req
    }
  }
  return false;  // need more data
}

void Controller::handleConnections(void) {
  // create just one pool of events, its like a line, if is a new conection, add
  // to poll, if not, handle the client conection.
  //struct epoll_event events[100];
  while (true) {
    int numEvents = epoll_wait(epollfd, events.data(), events.size(), -1);

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
        connectedClients[currentFd]->isReady = isHTTPRequestComplete(connectedClients[currentFd]->buffer);
      } else if ((currentEvent & EPOLLOUT) == EPOLLOUT) {
        if (connectedClients[currentFd]->isReady == true) {
          sendToClient(currentFd);
        }
      }
    }
    checkTimeOut();
  }
}

void Controller::checkTimeOut() {
  time_t currentTime = time(NULL);
  std::map<int, Client *>::iterator it = connectedClients.begin();
  std::map<int, Client *>::iterator ite = connectedClients.end();
  std::vector<int> clientsToDelete;

  for (; it != ite; ++it) {
    int connectionFd = it->first;
    Client *client = it->second;

    if (client != NULL && currentTime > client->getTimeout()) {
      std::cout << "client dead" << std::endl;
      clientsToDelete.push_back(connectionFd);
    }
  }

  for (size_t i = 0; i < clientsToDelete.size(); i++) {
    closeConnection(clientsToDelete[i]);
    std::cout << "Connection with FD -> " << clientsToDelete[i];
    std::cout << " close due to timeout" << std::endl;
  }
}

void Controller::addNewConnection(int socketFD) {
  struct epoll_event ev;
  struct sockaddr_in clientToAdd;
  socklen_t len = sizeof(clientToAdd);
  int newConnection = accept(socketFD, (struct sockaddr *)&clientToAdd, &len);

  if (newConnection < 0) {
    std::cerr << "Failed to grab new connection. errno: " << errno << std::endl;
    return;
  }

  initEpollEvent(&ev, EPOLLIN | EPOLLRDHUP | EPOLLOUT, newConnection);
  int serverPort = getSocketPort(newConnection);
  Server *server = serverPool[serverPort];
  time_t currentTime = time(NULL);

  if (setToNonBlock(newConnection)) {
    throw std::runtime_error ("Nonblock setup error");
  }

  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newConnection, &ev) == -1) {
    std::cerr << "Failed to add new connection to epoll. errno: ";
    std::cerr << errno << std::endl;
    close(newConnection);
    return;
  }

  Client *newClient = new Client(newConnection, server, serverPort, currentTime);
  connectedClients.insert(std::make_pair(newConnection, newClient));
  events.push_back(ev);
}

bool  Controller::closeConnection(int currentFd) {
  epoll_ctl(epollfd, EPOLL_CTL_DEL, currentFd, NULL);
  std::vector<struct epoll_event>::iterator it = events.begin();

  for (; it < events.end(); it++) {
    if (currentFd == it->data.fd) {
      events.erase(it);
      break;
    }
  }

  close(currentFd);
  delete connectedClients[currentFd];
  connectedClients.erase(currentFd);
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
  char buffer[4096];
  int bytesRead = recv(currentFd, buffer, 4096, 0);

  if (bytesRead < 0) {
    std::cout << " bytesread -1, will break" << errno << std::endl;
  } else if (bytesRead == 0) {
    std::cout << " bytesread 0, will close" << std::endl;
  } else if (bytesRead > 0) {
    std::cout << " i read -> " << bytesRead << " bytes" << std::endl;
    connectedClients[currentFd]->buffer.append(buffer, bytesRead);
  }
}

void Controller::sendToClient(int currentFd) {
  Client *client = connectedClients[currentFd];
  Server *server = client->getServer();
  client->getBuffer() += '\0';
  HttpResponse *response = server->process(client->getBuffer()); // alterar server pra receber string
  TCPServerSocket *socket = socketPool[client->getPort()];
  socket->sendData(currentFd, response->getHeaders().c_str(), response->getHeaders().size());
  socket->sendData(currentFd, response->getMsgBody(), response->getContentLength());

  // cleaning
  delete response;
  client->reset();

  //isso aqui deveria acontecer com keepalive ?
  closeConnection(currentFd);
}

int   setToNonBlock(int sock) {
  int flag = fcntl(sock, F_GETFL, 0);

  if (flag < 0)   {
    return (-1);
  }

  if (fcntl(sock, F_SETFL, flag | O_NONBLOCK) < 0)  {
    return (-1);
  }

  return (0);
}

void initEpollEvent(struct epoll_event *ev, uint32_t flag, int fd) {
  bzero(ev, sizeof(*ev));
  ev->events = flag;
  ev->data.fd = fd;
}

int Controller::getSocketPort(int socketFd) {
  struct sockaddr_in address;
  socklen_t sockAddrLen = sizeof(address);

  if (getsockname(socketFd, (struct sockaddr *)&address, &sockAddrLen) < 0) {
    std::cout << "Error trying to get socket name" << std::endl;
    return (-1);
  }

  int port = ntohs(address.sin_port);
  return (port);
}
