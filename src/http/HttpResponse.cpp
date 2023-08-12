/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:50:49 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/10 22:24:51 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpResponse.hpp"
#include "../../include/http/HttpTime.hpp"
#include "../../include/http/HttpStatus.hpp"

#include <sstream>

HttpResponse::HttpResponse(void) {
  statusCode = 200;
  statusMessage = "OK";
  contentLength = 0;
  serverVersion = "webserv/0.1";
}

HttpResponse::~HttpResponse(void) {}

std::string   HttpResponse::getHeaders(void) {
  std::stringstream ss;
  ss << statusCode;
  std::string responseHeader = protocol + " " + ss.str() + " " + statusMessage + "\n";
  responseHeader += "Server: " + serverVersion + "\n";
  responseHeader += "Date: ";
  responseHeader += HttpTime::getCurrentTime();

  if (!modifiedTime.empty()) {
    responseHeader += "Last-Modified: ";
    responseHeader += modifiedTime;
  }

  responseHeader += "Content-Type: " + contentType + "\n";
  ss.clear();
  ss.str("");
  ss << contentLength;
  responseHeader += "Content-length: " + ss.str() + "\n";
  responseHeader += "Connection: close\n\n";

  for (size_t i = 0; i < msgBody.size(); i++) {
    responseHeader += msgBody[i];
  }

  return responseHeader;
}

void HttpResponse::setProtocol(std::string protoName, int mainVer, int subVer) {
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

void HttpResponse::setMsgBody(const std::vector<char>&data) {
  msgBody = data;
}

void HttpResponse::setContentType(const std::string &mime) {
  contentType = mime;
}

void HttpResponse::setContentLength(size_t fileSize) {
  contentLength = fileSize;
}

void HttpResponse::setStatusCode(int httpCode) {
  statusCode = httpCode;
  statusMessage = HttpStatus::getMessage(httpCode);
}

void HttpResponse::setLastModifiedTime(std::string lmfTime) {
  modifiedTime = lmfTime;
}
