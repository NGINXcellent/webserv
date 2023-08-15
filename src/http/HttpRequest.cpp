/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:36:19 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/15 09:24:18 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequest.hpp"

HttpRequest::HttpRequest(void) {
  protocolMainVersion = -1;
  protocolSubVersion = -1;
}

void HttpRequest::setResponseStatusCode(int toset) {
  responseStatusCode = toset;
}

int HttpRequest::getResponseStatusCode() {
  return responseStatusCode;
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

void HttpRequest::setModifiedTimestampCheck(std::string timestamp) {
  modifiedTimestampCheck = timestamp;
}

std::string HttpRequest::getModifiedTimestampCheck(void) {
  return (this->modifiedTimestampCheck);
}

std::string HttpRequest::getUnmodifiedSinceTimestamp(void) {
  return (this->unmodifiedSinceTimestamp);
}

void HttpRequest::setUnmodifiedSinceTimestamp(std::string unmodifiedTimestamp) {
  unmodifiedSinceTimestamp = unmodifiedTimestamp;
}

std::vector<std::string> HttpRequest::getAllowedMethods(void) {
  return (this->allowedMethodList);
}

void HttpRequest::setAllowedMethods(std::vector<std::string> allowedList) {
  allowedMethodList = allowedList;
}
