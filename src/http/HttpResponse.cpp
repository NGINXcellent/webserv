/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:50:49 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/02 17:57:46 by lfarias-         ###   ########.fr       */
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
  std::string responseHeader = "HTTP/1.1 " + ss.str() + " " + statusMessage + "\n";
  responseHeader += "Server: " + serverVersion + "\n";
  responseHeader += "Date: ";
  responseHeader += HttpTime::getCurrentTime();
  responseHeader += "Content-Type: " + contentType + "\n";
  ss.clear();
  ss.str("");
  ss << msgBody.size();
  responseHeader += "Content-length: " + ss.str() + "\n";
  responseHeader += "Connection: close\n\n";

  for (size_t i = 0; i < msgBody.size(); i++) {
    responseHeader += msgBody[i];
  }

  return responseHeader;
}

void HttpResponse::setMsgBody(const std::vector<char>&data) {
  msgBody = data;
}

void HttpResponse::setContentType(const std::string &mime) {
  contentType = mime;
}

void HttpResponse::setStatusCode(int httpCode) {
  statusCode = httpCode;
  statusMessage = HttpStatus::getMessage(httpCode);
}
