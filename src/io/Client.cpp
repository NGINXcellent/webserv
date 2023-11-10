/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 16:10:03 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/10 06:32:07 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/Client.hpp"

#include "../../include/http/HttpTime.hpp"
#include "../../include/http/Server.hpp"

Client::Client(int conFd, servers_multimap serversL, int serverPort, time_t conStart, std::string kind) :
               connectionFd(conFd), port(serverPort), servers(serversL), \
               connectionTimeout(conStart + 60), kind(kind) {

  request = new HttpRequest();
  response = new HttpResponse();
  this->buffer = "";
  cgiClient = NULL;
  isReady = false;
  requestStatus = New_Status;
}

Client::Client(int conFd, servers_multimap serversL, int serverPort, time_t conStart, std::string kind, HttpRequest* request, HttpResponse* response) :
               connectionFd(conFd), port(serverPort), servers(serversL), \
               connectionTimeout(conStart + 60), request(request), response(response), kind(kind) {
  this->buffer = "";
  isReady = false;
  cgiClient = NULL;
  requestStatus = New_Status;
}


Client::~Client(void) {
  delete request;
  delete response;
  delete cgiClient;
}

std::string& Client::getBuffer(void) {
  return (this->buffer);
}

HttpRequest *Client::getRequest() {
  return (request);
}

HttpResponse *Client::getResponse() {
  return (response);
}

Server* Client::getServer(void) {
  return (this->server);
}

void Client::setServer(Server *newServer) {
  this->server = newServer;
}

int Client::getPort(void) {
  return (this->port);
}

void Client::reset() {
  buffer.clear();
    if (request != NULL) {
        request = NULL;
    }
    if (response != NULL) {
        response = NULL;
    }
  request = new HttpRequest();
  response = new HttpResponse();
  cgiClient = NULL;
  isReady = false;
}

time_t Client::getTimeout(void) {
  return (this->connectionTimeout);
}

std::string Client::getKind(void) {
  return (this->kind);
}

void Client::setCgiClient(Client* client) {
  this->cgiClient = client;
}

Client* Client::getCgiClient(void) {
  return (this->cgiClient);
}

void Client::setRequestStatus(HttpStatusCode status) {
  this->requestStatus = status;
}

HttpStatusCode Client::getRequestStatus(void) {
  return (this->requestStatus);
}

void Client::setKind(std::string kind) {
  this->kind = kind;
}

void Client::setBuffer(std::string toset) {
  this->buffer = toset;
}

int Client::getConnectionFd(void) {
  return (this->connectionFd);
}

void Client::chooseServer(const std::string& server_name) {
  for (std::multimap<int, Server*>::iterator it = servers.first; it != servers.second; ++it) {
    Server *current_server = it->second; 
    std::vector<std::string> &server_names = current_server->getServerNames(); 

    for (size_t i = 0; i < server_names.size(); i++) {
      if (server_names[i] == server_name) {
        this->server = current_server;
        break;
      }
    }
  }
}
