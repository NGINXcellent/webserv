/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:36:19 by lfarias-          #+#    #+#             */
/*   Updated: 2023/10/29 08:03:08 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequest.hpp"

#include <sstream>
#include <iostream>

HttpRequest::HttpRequest(void) {
  protocolMainVersion = -1;
  protocolSubVersion = -1;
  responseStatusCode = 0;
  hasHeader = false;
  requestReady = false;
  isCGI = false;
}

bool HttpRequest::isDirListActive(void) {
  return (this->autoindex);
}

void  HttpRequest::setDirListActive(const bool &active) {
    autoindex = active;
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

PostType HttpRequest::getPostType(void) {
  return (this->postType);
}

void HttpRequest::setPostType(PostType type) {
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

const MultiPartMap& HttpRequest::getMultipartMap(void) {
  return (multipartMap);
}

void HttpRequest::setMultipartMap(const MultiPartMap& parts) {
  multipartMap.clear();
  std::map<std::string, std::string>::const_iterator it = parts.begin();

  for (; it != parts.end(); ++it) {
      multipartMap[it->first] = it->second;
      // std::cout << it->first << " " << it->second << std::endl;
  }
}

const std::string &HttpRequest::getRoot(void) {
  return (root);
}

void HttpRequest::setRoot(const std::string &rootPath) {
  root = rootPath;
}
  void HttpRequest::setBaseLocation(std::string baseLocation){
    this->baseLocation = baseLocation;
  }

  std::string HttpRequest::getBaseLocation(void) {
    return baseLocation;
  }

void HttpRequest::setHeaderReady(bool opt) {
  hasHeader = opt;
}

bool HttpRequest::isHeaderReady(void) {
  return (hasHeader);
}

bool HttpRequest::isRequestReady(void) {
  return (requestReady);
}

void HttpRequest::setRequestReady(bool set) {
  requestReady = set;
}

void HttpRequest::setBodyNotParsed(std::string toset) {
  bodyNotParsed = toset;
}

std::string HttpRequest::getBodyNotParsed(void) {
  return bodyNotParsed;
}

void HttpRequest::setCGI(bool toset, std::string cgiPath, std::string cgiExtension) {
  this->cgiPath = cgiPath;
  this->cgiExtension = cgiExtension;

  isCGI = toset;
}

bool HttpRequest::getCGI(void) {
  return isCGI;
}

std::string HttpRequest::getCGIPath(void) {
  return cgiPath;
}

std::string HttpRequest::getCGIExtension(void) {
  return cgiExtension;
}

void HttpRequest::setQueryString(std::string toSet) {
  queryString = toSet;
}

std::string HttpRequest::getQueryString(void) {
  return queryString;
}

void HttpRequest::setContentType(std::string toset) {
  contentType = toset;
}

std::string HttpRequest::getContentType(void) {
  return contentType;
}