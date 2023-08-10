/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:22:33 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/10 08:32:50 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/Server.hpp"
#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/HttpResponseComposer.hpp"
#include "../../include/http/MimeType.hpp"
#include "../../include/socket/TcpServerSocket.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <sstream>  // stringstream

Server::Server(const struct s_serverConfig& config) {
  port = strtol(config.port.c_str(), NULL, 0);
  host = config.host;
  server_name = config.server_name;
  max_body_size = strtol(config.port.c_str(), NULL, 0);
  error_pages = config.error_page;
  locations = config.location;
}

Server::~Server(void) {
  delete socket;
}

void  Server::resolve(HttpRequest *request, HttpResponse *response) {
  // check the protocol version
  std::string requestMethod = request->getMethod();

  if (requestMethod == "GET")
    get(request, response);
  else if (requestMethod == "HEAD")
    head(request, response);
  else
    HttpResponseComposer::buildErrorResponse(response, 501, \
                       request->getProtocolMainVersion(), \
                       request->getProtocolSubVersion());
}

std::string Server::createLocation(char *buffer) {
    std::istringstream streaming(buffer);
    std::string line;
    streaming >> line;
    streaming >> line;
    std::vector<std::string> tokens;

    std::istringstream iss(line);
    std::string token;

    while (std::getline(iss, token, '/')) {
      if (!token.empty()) {
        token = '/' + token;
        tokens.push_back(token);
      }
    }
    for (size_t i = 0; i < locations.size(); ++i) {
      if (locations[i].location == tokens[0]) {
          if (locations[i].root.empty())
            return '.' + line;
          else {
            std::string ret;
            ret+='.' + locations[i].root;
             if(!locations[i].index.empty()) { // if we have index, this will always serve root + index content
               ret+='/' + locations[i].index;
               return ret;
             }
            for (size_t j = 1; j < tokens.size(); ++j) {
              ret += tokens[j];
            }
            return ret;
          }
      }
    }
    return "error";
}

std::string Server::process(char *buffer) {
  std::string location = createLocation(buffer);
  std::cout << "print da func: " << location << std::endl;
  HttpRequest *request = HttpRequestFactory::createFrom(buffer,location);
  HttpResponse response;

  int status = HttpRequestFactory::check(request);

  if (status != 0) {
    HttpResponseComposer::buildErrorResponse(&response, \
                                          status, \
                                          request->getProtocolMainVersion(), \
                                          request->getProtocolSubVersion());
  } else {
    resolve(request, &response);
  }

  delete request;
  return response.getHeaders();
}

void Server::get(HttpRequest *request, HttpResponse *response) {
  std::ifstream inputFile;
  int protoMain = request->getProtocolMainVersion();
  int protoSub = request->getProtocolSubVersion();

  if (access(request->getResource().c_str(), F_OK) == -1) {
    HttpResponseComposer::buildErrorResponse(response, \
                                             404, protoMain, protoSub);
    return;
  }

  if (access(request->getResource().c_str(), R_OK) == -1) {
    HttpResponseComposer::buildErrorResponse(response, \
                                             403, protoMain, protoSub);
    return;
  }

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
  response->setProtocol("HTTP", protoMain, protoSub);
  response->setContentType(MimeType::identify(request->getResource()));
  response->setMsgBody(resourceData);
  response->setContentLength(resourceData.size());
}

void Server::head(HttpRequest *request, HttpResponse *response) {
  this->get(request, response);
  std::vector<char> empty;
  response->setMsgBody(empty);
}

int   Server::getPort(void) {
  return (port);
}

