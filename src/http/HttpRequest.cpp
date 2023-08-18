/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:36:19 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/18 17:49:50 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequest.hpp"

HttpRequest::HttpRequest(void) {
  protocolMainVersion = -1;
  protocolSubVersion = -1;
  responseStatusCode = 0;
}

void HttpRequest::setResponseStatusCode(int toset) {
  responseStatusCode = toset;
}

int HttpRequest::getResponseStatusCode() {
  return responseStatusCode;
}

HttpRequest::~HttpRequest(void) {}

const std::string &HttpRequest::getResource(void) {
  return (this->resource);
}

void HttpRequest::setResource(const std::string &resource) {
  this->resource = resource;
}

const std::string &HttpRequest::getMethod(void) {
  return (this->method);
}

void HttpRequest::setMethod(const std::string &method) {
  this->method = method;
}

const std::string &HttpRequest::getProtocolName(void) {
  return (this->protocolName);
}

void HttpRequest::setProtocolName(const std::string &protocol) {
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

const std::string &HttpRequest::getHost(void) {
  return (this->host);
}

void HttpRequest::setHost(const std::string &nHost) {
  host = nHost;
}

void HttpRequest::setModifiedTimestampCheck(const std::string &timestamp) {
  modifiedTimestampCheck = timestamp;
}

const std::string &HttpRequest::getModifiedTimestampCheck(void) {
  return (this->modifiedTimestampCheck);
}

const std::string &HttpRequest::getUnmodifiedSinceTimestamp(void) {
  return (this->unmodifiedSinceTimestamp);
}

void HttpRequest::setUnmodifiedSinceTimestamp(const std::string &unmodifiedTimestamp) {
  unmodifiedSinceTimestamp = unmodifiedTimestamp;
}

const std::vector<std::string> &HttpRequest::getAllowedMethods(void) {
  return (this->allowedMethodList);
}

void HttpRequest::setAllowedMethods(const std::vector<std::string> &allowedList) {
  allowedMethodList = allowedList;
}
