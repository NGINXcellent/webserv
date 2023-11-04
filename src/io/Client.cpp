/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 16:10:03 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/04 07:59:56 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/Client.hpp"

#include "../../include/http/HttpTime.hpp"
#include "../../include/http/Server.hpp"

Client::Client(int conFd, Server *destServer, int serverPort, time_t conStart, std::string kind) :
               connectionFd(conFd), port(serverPort), server(destServer), \
               connectionTimeout(conStart + 60), kind(kind) {
  request = new HttpRequest();
  response = new HttpResponse();
  this->buffer = "";
  cgiClient = NULL;
  isReady = false;
  requestStatus = New_Status;
}

Client::Client(int conFd, Server *destServer, int serverPort, time_t conStart, std::string kind, HttpRequest* request, HttpResponse* response) :
               connectionFd(conFd), port(serverPort), server(destServer), \
               connectionTimeout(conStart + 60), request(request), response(response), kind(kind) {
  this->buffer = "";
  isReady = false;
}


Client::~Client(void) {
  delete request;
  delete response;
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

int Client::getPort(void) {
  return (this->port);
}

void Client::reset() {
  buffer.clear();
  delete request;
  delete response;
  request = new HttpRequest();
  response = new HttpResponse();
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