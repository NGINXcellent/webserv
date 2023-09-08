/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 16:10:03 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/05 21:12:23 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/io/Client.hpp"

#include "../../include/http/HttpTime.hpp"
#include "../../include/http/Server.hpp"

Client::Client(int conFd, Server *destServer, int serverPort, time_t conStart) :
               connectionFd(conFd), port(serverPort), server(destServer), \
               connectionTimeout(conStart + 60) {
  request = new HttpRequest();
  response = new HttpResponse();
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

