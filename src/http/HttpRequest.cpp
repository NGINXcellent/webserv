/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:36:19 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/02 20:32:30 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequest.hpp"

HttpRequest::HttpRequest(void) {}

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

std::string HttpRequest::getProtocolVersion(void) {
  return (this->protocolVersion);
}

void HttpRequest::setProtocolVersion(std::string protocol) {
  protocolVersion = protocol;
}

std::string HttpRequest::getHost(void) {
  return (this->host);
}

void HttpRequest::setHost(std::string nHost) {
  host = nHost;
}
