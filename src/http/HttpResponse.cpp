/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:50:49 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/02 15:31:29 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpResponse.hpp"
#include "../../include/http/HttpTime.hpp"
#include "../../include/http/HttpStatus.hpp"

#include <sstream>

HttpResponse::HttpResponse(void) {
  statusCode = Ok;
  statusMessage = "OK";
  contentLength = 0;
  serverVersion = "webserv/0.1";
  msgBody = NULL;
}

HttpResponse::~HttpResponse(void) {
  if (msgBody != NULL) {
    delete[] msgBody;
  }
}

std::string   HttpResponse::getHeaders(void) {
  std::stringstream ss;
  ss << statusCode;
  std::string responseHeader = protocol + " " + ss.str() + " " + statusMessage + "\n";
  responseHeader += "Server: " + serverVersion + "\n";
  responseHeader += "Date: ";
  responseHeader += HttpTime::getCurrentTime();

  if (!location.empty()) {
    responseHeader += "Location: ";
    responseHeader += location + "\n";
  }

  if (!modifiedTime.empty()) {
    responseHeader += "Last-Modified: ";
    responseHeader += modifiedTime;
  }

  if (msgBody != NULL) {
    responseHeader += "Content-Type: " + contentType + "\n";
    ss.clear();
    ss.str("");
    ss << contentLength;

    responseHeader += "Content-length: " + ss.str() + "\n";
  }

  responseHeader += "Connection: close\n\n";
  return responseHeader;
}

void HttpResponse::setProtocol(const std::string &protoName,
                                    int mainVer, int subVer) {
  std::stringstream ss;
  std::string stringProtocol = protoName + "/";
  ss << mainVer;
  stringProtocol += ss.str() + ".";
  ss.clear();
  ss.str("");
  ss << subVer;
  stringProtocol += ss.str();
  this->protocol = stringProtocol;
}

void HttpResponse::setMsgBody(char *resourceData) {
  msgBody = resourceData;
}

char *HttpResponse::getMsgBody(void) {
  return msgBody;
}

void HttpResponse::setContentType(const std::string &mime) {
  contentType = mime;
}

void HttpResponse::setContentLength(long long fileSize) {
  contentLength = fileSize;
}

void HttpResponse::setLocation(const std::string &loc) {
  location = loc;
}

long long HttpResponse::getContentLength(void) {
  return contentLength;
}

void HttpResponse::setStatusCode(int httpCode) {
  statusCode = httpCode;
  statusMessage = HttpStatus::getMessage(httpCode);
}

void HttpResponse::setLastModifiedTime(const std::string &lmfTime) {
  modifiedTime = lmfTime;
}
