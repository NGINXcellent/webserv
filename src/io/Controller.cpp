/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Controller.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:51:31 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/10 07:52:23 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/Controller.hpp"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <iostream>

#include "../../include/io/TcpServerSocket.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/utils/Logger.hpp"
#include "../../include/http/Server.hpp"

Controller::Controller(const InputHandler &input) {
  defaultServer = NULL;
  std::vector<struct s_serverConfig>::iterator it = input.serverVector->begin();
  std::vector<struct s_serverConfig>::iterator end = input.serverVector->end();

  bool is_first = true;

  while (it != end) {
    const struct s_serverConfig &serverConfig = *it;
    Server *newServer = new Server(serverConfig);
    std::vector<size_t> ports = newServer->getPorts();

    for (size_t i = 0; i < ports.size(); i++) {
      this->serverPool.insert(std::make_pair(ports[i], newServer));
    }

    if (is_first) {
      this->defaultServer = newServer;
      is_first = false;
    }

    serverList.push_back(newServer);
    ++it;
  }

  events.reserve(EPOLL_SIZE);
}

Controller::~Controller(void) {
  std::map<int, Client*>::iterator clientIt = connectedClients.begin();
  std::map<int, Client*>::iterator clientIte = connectedClients.end();

  for (; clientIt != clientIte; clientIt++) {
    closeConnection(clientIt->first);
  }
  
  for (std::multimap<int, Server*>::iterator it = serverPool.begin(); it != serverPool.end(); /* no increment here */) {
      // Get the current key
      int currentKey = it->first;

      // Find the range of elements with the current key
      std::pair<std::multimap<int, Server*>::iterator, std::multimap<int, Server*>::iterator> range;
      range = serverPool.equal_range(currentKey);

      // Delete Server* objects associated with the current key
      for (std::multimap<int, Server*>::iterator delIt = range.first; delIt != range.second; ++delIt) {
          delete delIt->second; // Delete the Server* object
      }

      // Erase the range of elements
      serverPool.erase(range.first, range.second);

      // Move the iterator to the next key
      it = serverPool.upper_bound(currentKey);
  }

  std::map<int, TCPServerSocket*>::iterator socketIt = socketPool.begin();
  std::map<int, TCPServerSocket*>::iterator socketIte = socketPool.end();

  for (; socketIt != socketIte; socketIt++) {
    close(socketIt->second->getFD());
    delete socketIt->second;
  }

  close(epollfd);
}

void Controller::endServer() {
  exit(0);
}

void Controller::signalHandler(int signal) {
  if (signal == SIGINT) {
    Logger::msg << "\nServer is stopping gracefully, goodbye !";
    Logger::print(Info);
    endServer();
  }
}

void Controller::init(void) {
  //  Create epollfd
  std::signal(SIGINT, signalHandler);
  struct epoll_event ev;
  epollfd = epoll_create(EPOLL_SIZE);

  if (epollfd == -1) {
    Logger::msg << "Failed to create epoll. errno: " << errno;
    Logger::print(Error);
    exit(EXIT_FAILURE);
  }
  
  // start sockets
  //std::map<int, Server*>::iterator it = serverPool.begin();
  //std::map<int, Server*>::iterator ite = serverPool.end();

  for (size_t i = 0; i < serverList.size(); i++) {
    serverList[i]->setControllerPtr(this);
    std::vector<size_t> ports = serverList[i]->getPorts();

    for (size_t j = 0; j < ports.size(); j++) {
      int port = ports[j];
      std::map<int, TCPServerSocket*>::iterator it = socketPool.find(port);

      if (it != socketPool.end()) { // port already binded
        continue;
      }

      TCPServerSocket *socket = new TCPServerSocket(port, serverList[i]->getHost());

      if (socket->bindAndListen() == -1) {
        Logger::msg << "webserv will not be able to serve pages on port " << port;
        Logger::print(Warning);
        continue;
      }

      socketPool.insert(std::make_pair(port, socket));
      Logger::msg << "listening on " << serverList[i]->getHost() << ":" << socket->getPort();
      Logger::print(Info);

      // Bind sockfd on epoll event
      initEpollEvent(&ev, EPOLLIN | EPOLLOUT, socket->getFD());

      if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket->getFD(), &ev) == -1) {
        Logger::msg << "Failed to add sockfd to epoll. errno: " << errno;
        Logger::print(Error);
      } else {
        events.push_back(ev);
      }
    }
  }

  handleConnections();
}

void Controller::handleConnections(void) {
  // create just one pool of events, its like a line, if is a new conection, add
  // to poll, if not, handle the client conection.
  // struct epoll_event events[100];
  while (true) {
    int numEvents = epoll_wait(epollfd, events.data(), EPOLL_SIZE, -1);

    if (numEvents == -1) {
      Logger::msg << "epoll_wait error. errno: " << errno;
      Logger::print(Error);
      continue;
    }

    for (int i = 0; i < numEvents; ++i) {
      int currentFd = events[i].data.fd;    // this client fd
      int currentEvent = events[i].events;  // this client event state
      Client *client = connectedClients[currentFd];
      Logger::msg << "FD in Line: " << currentFd;
      Logger::print(Info);
      if (isNewConnection(currentFd) && client == NULL ) {
        addNewConnection(currentFd, "CLIENT");  // if new connection found, add it.
      } else if ((currentEvent & EPOLLRDHUP) == EPOLLRDHUP) {
        Logger::msg << "Connection with FD -> " << currentFd;
        Logger::msg << " is closed by client";
        Logger::print(Info);
        closeConnection(currentFd);
      } else if ((currentEvent & EPOLLIN) == EPOLLIN) {
        Client *client = connectedClients[currentFd];
        if (client != NULL && client->getKind() == "CGI") {
          client->setBuffer(readFromPipe(currentFd));
          client->setRequestStatus(Ready);
          removeFromLine(currentFd);
        }

        if (client != NULL && client->getKind() == "CLIENT") {
          if (readFromClient(currentFd) < 0) {
            Logger::msg << " error reading from client: " << errno;
            Logger::msg << " on port: " << client->getPort() <<". disconnecting";
            Logger::print(Error);
            closeConnection(currentFd);
          } else {
            HttpRequest *request = client->getRequest();
            std::string &clientBuffer = client->getBuffer();
            request->setRequestReady(isHTTPRequestComplete(request, clientBuffer));
          }
        }
      } else if ((currentEvent & EPOLLOUT) == EPOLLOUT) {
        Client *client = connectedClients[currentFd];
        HttpRequest *request = client->getRequest();
        HttpResponse *response = client->getResponse();

        if(client->getKind() == "CLIENT") {

          if (request->isRequestReady() && client->getRequestStatus() == New_Status){
              client->chooseServer(request->getServerName());

              if (client->getServer() == NULL) {
                client->setServer(defaultServer);
              } 

              HttpStatusCode status;
              status = client->getServer()->process(client, request, response);
              client->setRequestStatus(status);
          }

          if(client->getCgiClient() != NULL) {
            if (client->getCgiClient()->getRequestStatus() == Ready)
              client->setRequestStatus(Ready);
          }

          if (client != NULL && client->getRequestStatus() == Ready) {
            sendToClient(currentFd);
          }
        }
      }
    }
    // checkTimeOut();
  }
}

bool Controller::isChunkedBodyComplete(const std::string &body) {
  size_t pos = body.find("\r\n0\r\n\r\n", body.size() - 7);
  return (pos != std::string::npos);
}

bool Controller::isMultipartBodyComplete(const std::string &body) {
  size_t pos = body.find("--\r\n", body.size() - 6);
  return (pos != std::string::npos);
}

bool Controller::isUrlEncodedBodyComplete(const std::string &body, size_t cLength) {
  return (body.size() == cLength);
}

std::string toLowerStr2(std::string str) {
  std::string result;

  for (size_t i = 0; i < str.size(); i++) {
    result += static_cast<char>(std::tolower(str[i]));
  }

  return (result);
}

bool Controller::isHTTPRequestComplete(HttpRequest *request, std::string &requestMsg) {
  if (request->isHeaderReady() && request->getMethod() != "POST") {
      return (true);
  }

  size_t pos = requestMsg.find("\r\n\r\n");

  if (pos == std::string::npos) {
    return (false);
  }

  if (!request->isHeaderReady()) {
    if (request->getHost().empty())
      HttpRequestFactory::setupHeader(request, requestMsg);
    // we need to make sure that header is ready before enter POST check.
    // i`m assuming that the header is always ready after first interaction.
    // whe should change this.
    if(request->getMethod() != "POST")
      request->setHeaderReady(true);

    if (request->getMethod() == "POST") {
      size_t contentPos = requestMsg.find("\r\n\r\n") + 4;
      std::string body = requestMsg.substr(contentPos);
      if (body == "") {
        return request->isHeaderReady();
      }

      PostType pType = request->getPostType();
      size_t cLenght = request->getContentLength();
      switch(pType) {
        case None:
          return false;

        case Chunked:
          request->setHeaderReady(isUrlEncodedBodyComplete(body, cLenght));
          return request->isHeaderReady();

        case Multipart:
          request->setHeaderReady(isUrlEncodedBodyComplete(body, cLenght));
          return request->isHeaderReady();

        case UrlEncoded:
          request->setHeaderReady(isUrlEncodedBodyComplete(body, cLenght));
          return request->isHeaderReady();
      }
    }
  }

  return request->isHeaderReady();
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
      clientsToDelete.push_back(connectionFd);
    }
  }

  for (size_t i = 0; i < clientsToDelete.size(); i++) {
    closeConnection(clientsToDelete[i]);
    Logger::msg << "Connection with FD -> " << clientsToDelete[i];
    Logger::msg << " close due to timeout";
    Logger::print(Info);
  }
}

void Controller::addCGItoEpoll(int fd, int port, Client* client) {
  struct epoll_event ev;
  initEpollEvent(&ev, EPOLLIN, fd);
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    Logger::msg << "Failed to add new connection to epoll. errno: " << errno;
    Logger::print(Error);
    close(fd);
    return;
  }


  serverList servers;
  Client *newClient = new Client(fd, servers, port, 100000000, "CGI", NULL, NULL);
  newClient->setRequestStatus(CGI);
  client->setCgiClient(newClient);
  connectedClients.insert(std::make_pair(fd, newClient));
  events.push_back(ev);
}

void Controller::addNewConnection(int socketFD, std::string kind) {
  struct epoll_event ev;
  struct sockaddr_in clientToAdd;
  socklen_t len = sizeof(clientToAdd);
  int newConnection = accept(socketFD, (struct sockaddr *)&clientToAdd, &len);

  if (newConnection < 0) {
    Logger::msg << "Failed to grab new connection. errno: " << errno;
    Logger::print(Error);
    return;
  }

  initEpollEvent(&ev, EPOLLIN | EPOLLRDHUP | EPOLLOUT, newConnection);
  int serverPort = getSocketPort(newConnection);
  
  std::pair<std::multimap<int, Server*>::iterator, std::multimap<int, Server*>::iterator> servers = serverPool.equal_range(serverPort);
  time_t currentTime = time(NULL);

  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newConnection, &ev) == -1) {
    Logger::msg << "Failed to add new connection to epoll. errno: " << errno;
    Logger::print(Error);
    close(newConnection);
    return;
  }

  Client *newClient = new Client(newConnection, servers, serverPort, currentTime, kind);
  connectedClients.insert(std::make_pair(newConnection, newClient));
  events.push_back(ev);
}

void Controller::removeFromLine(int currentFd) {
  epoll_ctl(epollfd, EPOLL_CTL_DEL, currentFd, NULL);
  close(currentFd);
}

void Controller::removeFromConnectedCLients(int currentFd) {
  if (connectedClients[currentFd] != NULL) {
    delete connectedClients[currentFd];
  }

  connectedClients.erase(currentFd);
}

bool  Controller::closeConnection(int currentFd) {
  epoll_ctl(epollfd, EPOLL_CTL_DEL, currentFd, NULL);
  close(currentFd);

  if (connectedClients[currentFd] != NULL) {
    delete connectedClients[currentFd];
  }

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

std::string Controller::readFromPipe(int currentFd) {
  std::string toret;
  int tmp = 4096;
  char buf[tmp];
  int strlen;
  
  while (42) {
    strlen = read(currentFd, buf, tmp);
    buf[strlen] = 0;
    if (strlen == 0) {
      // Logger::msg << "Pipe has been empty!";
      // Logger::print(Debug);
      break;
    } else if (strlen > 0) {
      toret.append(buf);
    } else {
      Logger::msg << "Pipe read error!";
      Logger::print(Debug);
      break;
    }
  }
  return toret;
}

int Controller::readFromClient(int currentFd) {
  int bytesRead = read(currentFd, buffer, 4096);
  if (bytesRead == 0) {
    Logger::msg << "Read 0 bytes!";
    Logger::print(Debug);
  }
  
  if (bytesRead > 0 && connectedClients[currentFd] != NULL) {
      connectedClients[currentFd]->buffer.append(buffer, bytesRead);
  }

  return (bytesRead);
}

void Controller::sendToClient(int currentFd) {
  Client *client = connectedClients[currentFd];
  HttpResponse *response = client->getResponse();
  TCPServerSocket *socket = socketPool[client->getPort()];

  client->getBuffer() += '\0';
  if (client->getCgiClient() != NULL) {
    if (!client->getCgiClient()->getBuffer().empty()) {
      client->getCgiClient()->getBuffer() += '\0';
      const char *bodyData = client->getCgiClient()->getBuffer().c_str();
      char *bodyCopy = new char[strlen(bodyData) + 1];
      strncpy(bodyCopy, bodyData, strlen(bodyData) + 1);
      response->setMsgBody(bodyCopy);
      response->setContentLength(strlen(bodyCopy));
      response->setContentType("text/html");
      int s_status = socket->sendData(currentFd, response->getHeaders().c_str(),
                       response->getHeaders().size());
      int s_status_1 = socket->sendData(currentFd, response->getMsgBody(),
                       response->getContentLength());

      if (s_status < 0 || s_status_1 < 0) {
        Logger::msg << " error while sending data to client: " << currentFd;
        Logger::msg << " on port " << client->getPort() << ". disconneting";
      }
      
      closeConnection(client->getCgiClient()->getConnectionFd());
      client->reset();
      closeConnection(currentFd);
    }
  } else {
    int sendStatus = socket->sendData(currentFd, response->getHeaders().c_str(), \
                     response->getHeaders().size());
    int sendStatus_1 = socket->sendData(currentFd, response->getMsgBody(), \
                     response->getContentLength());

    if (sendStatus < 0 || sendStatus_1 < 0) {
      Logger::msg << " error while sending data to client: " << currentFd;
      Logger::msg << " on port " << client->getPort() << ". disconneting";
    }
 
    client->reset();
    closeConnection(currentFd);
  }
}

void Controller::initEpollEvent(struct epoll_event *ev, uint32_t flag, int fd) {
  bzero(ev, sizeof(*ev));
  ev->events = flag;
  ev->data.fd = fd;
}

int Controller::getSocketPort(int socketFd) {
  struct sockaddr_in address;
  socklen_t sockAddrLen = sizeof(address);

  if (getsockname(socketFd, (struct sockaddr *)&address, &sockAddrLen) < 0) {
    Logger::msg << "Error trying to get socket name";
    Logger::print(Error);
    return (-1);
  }

  int port = ntohs(address.sin_port);
  return (port);
}
