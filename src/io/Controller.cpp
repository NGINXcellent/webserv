/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Controller.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:51:31 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/01 12:39:59 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/Controller.hpp"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <iostream>

#include "../../include/io/TcpServerSocket.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/Server.hpp"

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
    it->second->setControllerPtr(this);
    int port = it->second->getPort();
    TCPServerSocket *socket = new TCPServerSocket(port);
    socket->bindAndListen();
    socketPool.insert(std::make_pair(port, socket));
    std::cout << "[LOG]\tlistening on port: " << socket->getPort() << std::endl;

    // Bind sockfd on epoll event
    initEpollEvent(&ev, EPOLLIN | EPOLLOUT, socket->getFD());

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
        addNewConnection(currentFd, "CLIENT");  // if new connection found, add it.
      } else if ((currentEvent & EPOLLRDHUP) == EPOLLRDHUP) {
        std::cout << "Connection with FD -> " << currentFd;
        std::cout << " is closed by client" << std::endl;
        closeConnection(currentFd);
      } else if ((currentEvent & EPOLLIN) == EPOLLIN) {
        Client *client = connectedClients[currentFd];
        if (client->getKind() == "CLIENT") {
        readFromClient(currentFd);
        HttpRequest *request = connectedClients[currentFd]->getRequest();
        std::string &clientBuffer = connectedClients[currentFd]->getBuffer();
        // DEBUG CLIENT BUFFER
        // std::cout << clientBuffer << std::endl;
        request->setRequestReady(isHTTPRequestComplete(request, clientBuffer));
        }
        if (client->getKind() == "CGI") {
          std::cout << "OLA EU SOU UM CGI E PASSEI POR AQUI" << std::endl;
          readFromClient(currentFd);
        }
      } else if ((currentEvent & EPOLLOUT) == EPOLLOUT) {
        Client *client = connectedClients[currentFd];
        if(client->getKind() == "CLIENT") {
        if (client != NULL && client->getRequest()->isRequestReady()) {
          // DEBUG CLIENT BUFFER
          // std::cout << connectedClients[currentFd]->getBuffer() << std::endl;
          sendToClient(currentFd);
        }
        }
        if (client->getKind() == "CGI") {
          std::cout << "TECNICAMENTE EU ACABEI DE LER MEU FD INTEIRO" << std::endl;
          std::cout<< client->getBuffer() << std::endl;
          sendCgiToClient(currentFd);
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

// we already have an toLowerStr, need to remove this . . .
std::string toLowerStr2(std::string str) {
  std::string result;

  for (size_t i = 0; i < str.size(); i++) {
    result += static_cast<char>(std::tolower(str[i]));
  }

  return (result);
}

bool Controller::isHTTPRequestComplete(HttpRequest *request, std::string &requestMsg) {
  // Esta entrando aqui duas vezes e tirando o host
  // possivelmente vamos dividir header e body e só passar quando os dois estiverem prontos.
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
    // this is important, idk why but it is.
    size_t contentPos = requestMsg.find("\r\n\r\n") + 4;
    //  DEBUG
    // std::cout << contentPos << std::endl;
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

void Controller::addCGItoEpoll(int fd, Server* serv, int port, HttpRequest* req, HttpResponse* res) {
  struct epoll_event ev;
  initEpollEvent(&ev, EPOLLIN | EPOLLRDHUP | EPOLLOUT, fd);
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    std::cerr << "Failed to add new connection to epoll. errno: ";
    std::cerr << errno << std::endl;
    close(fd);
    return;
  }
  Client *newClient = new Client(fd, serv, port, 0, "CGI", req, res);
  connectedClients.insert(std::make_pair(fd, newClient));
  events.push_back(ev);

}

void Controller::addNewConnection(int socketFD, std::string kind) {
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

  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newConnection, &ev) == -1) {
    std::cerr << "Failed to add new connection to epoll. errno: ";
    std::cerr << errno << std::endl;
    close(newConnection);
    return;
  }

  Client *newClient = new Client(newConnection, server, serverPort, currentTime, kind);
  connectedClients.insert(std::make_pair(newConnection, newClient));
  events.push_back(ev);
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

void Controller::readFromClient(int currentFd) {
  int bytesRead = read(currentFd, buffer, 4096);

  if (bytesRead < 0) {
    std::cout << " bytesread -1, will break" << errno << std::endl;
  } else if (bytesRead == 0) {
    std::cout << " bytesread 0, will close" << std::endl;
  } else if (bytesRead > 0) {
    // DEBUG read count epollin
    // std::cout << " i read -> " << bytesRead << " bytes" << std::endl;
    if(connectedClients[currentFd] != NULL) {
      connectedClients[currentFd]->buffer.append(buffer, bytesRead);
    }
  }
}

void Controller::sendToClient(int currentFd) {
  Client *client = connectedClients[currentFd];
  // std::cout << client->getBuffer() << std::endl;
  Server *server = client->getServer();
  HttpRequest *request = client->getRequest();
  HttpResponse *response = client->getResponse();
  TCPServerSocket *socket = socketPool[client->getPort()];
  HttpStatusCode status;

  client->getBuffer() += '\0';
  status = server->process(client->getBuffer(), request, response);
  if (status == Ready) {
  socket->sendData(currentFd, response->getHeaders().c_str(), \
                   response->getHeaders().size());
  socket->sendData(currentFd, response->getMsgBody(), \
                   response->getContentLength());
  // DEBUG
  // std::cout << response->getHeaders() << std::endl;
  client->reset();
  closeConnection(currentFd);
  }
  if (status == CGI) {
    // client->reset();
    // closeConnection(currentFd);
  }
}

void Controller::sendCgiToClient(int currentFd) {
  Client *client = connectedClients[currentFd];
  // std::cout << client->getBuffer() << std::endl;
  // Server *server = client->getServer();
  // HttpRequest *request = client->getRequest();
  HttpResponse *response = client->getResponse();
  TCPServerSocket *socket = socketPool[client->getPort()];
    std::string cgiOutput = client->getBuffer();

    // std::cout << cgiOutput << std::endl;
    // if (cgiOutput.empty()) {
    //   return (No_Content);
    // }
    const char *bodyData = cgiOutput.c_str();  // Converter para const char*
    char *bodyCopy = new char[strlen(bodyData) + 1];
    strncpy(bodyCopy, bodyData, strlen(bodyData) + 1);
    response->setMsgBody(bodyCopy);
    response->setContentLength(strlen(bodyCopy));
    response->setContentType("text/html");
    socket->sendData(currentFd, response->getHeaders().c_str(), \
                   response->getHeaders().size());
    socket->sendData(currentFd, response->getMsgBody(), \
                   response->getContentLength());
    client->reset();
    closeConnection(currentFd);
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
    std::cout << "Error trying to get socket name" << std::endl;
    return (-1);
  }

  int port = ntohs(address.sin_port);
  return (port);
}
