/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:36:19 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/03 21:09:09 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequest.hpp"

HttpRequest::HttpRequest(void) {
  protocolMainVersion = 1;
  protocolSubVersion = 1;
}

HttpRequest::~HttpRequest(void) {}

std::string HttpRequest::getResource(void) {
  return (this->resource);
}

void HttpRequest::setResource(std::string resource) {
  this->resource = resource;
}

std::string HttpRequest::getMethod(void) {
  return (this->method);
}

void HttpRequest::setMethod(std::string method) {
  this->method = method;
}

std::string HttpRequest::getProtocolName(void) {
  return (this->protocolName);
}

void HttpRequest::setProtocolName(std::string protocol) {
  protocolName = protocol;
}


int HttpRequest::getProtocolMainVersion(void) {
  return (this->protocolMainVersion);
}

int HttpRequest::getProtocolSubVersion(void) {
  return (this->protocolSubVersion);
}

void HttpRequest::setProtocolVersion(int main, int sub) {
  protocolMainVersion = main;
  protocolSubVersion = sub;
}

std::string HttpRequest::getHost(void) {
  return (this->host);
}

void HttpRequest::setHost(std::string nHost) {
  host = nHost;
}
