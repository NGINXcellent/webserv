/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 16:10:03 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/21 01:50:40 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/socket/Client.hpp"
#include "../../include/http/HttpTime.hpp"
#include "../../include/http/Server.hpp"

Client::Client(int conFd, Server *destServer, int serverPort, time_t conStart) :
               connectionFd(conFd), port(serverPort), server(destServer), \
               connectionTimeout(conStart + 60) {
  //shim
  buffer.reserve(1024);
}

Client::~Client(void) {}

std::vector<char>& Client::getBuffer(void) { 
  return (this->buffer); 
}

Server* Client::getServer(void) {
  return (this->server);
}

int Client::getPort(void) {
  return (this->port);
}

void Client::reset() {
  std::vector<char>().swap(buffer);  // release memory and clean buffer
}

time_t Client::getTimeout(void) {
  return (this->connectionTimeout);
}
