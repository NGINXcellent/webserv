/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:36:19 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/28 19:58:57 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequest.hpp"

#include <sstream>

HttpRequest::HttpRequest(void) {
  protocolMainVersion = -1;
  protocolSubVersion = -1;
  responseStatusCode = 0;
}

bool HttpRequest::isDirListActive(void) {
  return (this->autoindex);
}

void  HttpRequest::setDirListActive(const std::string& active) {
  if (active == "on") {
    autoindex = true;
  }
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

const std::string& HttpRequest::getPostType(void) {
  return (this->postType);
}

void HttpRequest::setPostType(const std::string &type) {
  postType = type;
}

const std::string& HttpRequest::getRequestBody(void) {
  return (requestBody);
}

void HttpRequest::setRequestBody(const std::string &body) {
  requestBody = body;
}

const std::string& HttpRequest::getIndexPath(void) {
  return (indexPath);
}

void HttpRequest::setIndexPath(const std::string &iPath) {
  indexPath = iPath;
}

const std::string& HttpRequest::getRedirectionPath(void) {
  return (redirectionPath);
}

void HttpRequest::setRedirectionPath(const std::string &path) {
  redirectionPath = path; 
}

void HttpRequest::setRedirectionCode(int toset) {
  responseStatusCode = toset;
}

int HttpRequest::getRedirectionCode() {
  return responseStatusCode;
}

const std::string& HttpRequest::getLocationWithoutIndex(void) {
  return (this->locationWithoutIndex);
}

void HttpRequest::setLocationWithoutIndex(const std::string &locWithoutIndex) {
  this->locationWithoutIndex = locWithoutIndex;
}

size_t HttpRequest::getContentLength(void) {
  return (bodySize);
}

void HttpRequest::setContentLength(const std::string& sizeStr) {
  if (sizeStr.empty()) {
    bodySize = 0;
  }

  size_t number;
  std::stringstream ss(sizeStr);
  ss >> number;

  if (!ss.fail()) {
    char leftover;
    ss >> leftover;

    if (!ss.fail()) {
      bodySize = -1;
    } else {
      bodySize = number;
    }
  } else {
    bodySize = -1;
  }
}

const std::string& HttpRequest::getBoundary() {
 return boundary;
}

void HttpRequest::setBoundary(const std::string& boundary) {
 this->boundary = boundary;
}
const std::vector<s_multipartStruct>& HttpRequest::getMultipartStruct() {
 return multipartStructVector;
}

void HttpRequest::setMultipartStruct(const std::vector<s_multipartStruct>& parts ) {
 multipartStructVector.clear();
 multipartStructVector.insert(multipartStructVector.end(), parts.begin(), parts.end());
}

const std::string &HttpRequest::getRoot(void) {
  return (root);
}

void HttpRequest::setRoot(const std::string &rootPath) {
  root = rootPath;
}
