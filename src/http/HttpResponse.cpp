/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:50:49 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/01 23:10:20 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpResponse.hpp"
#include "../../include/http/HttpTime.hpp"

#include <sstream>

HttpResponse::HttpResponse(void) {
  statusCode = 200;
  statusMessage = "OK";
  contentLength = 0;
  serverVersion = "Server: webserv/0.1";
}

HttpResponse::~HttpResponse(void) {}

std::string   HttpResponse::getHeaders(void) {
  std::stringstream ss;
  ss << statusCode;
  std::string responseHeader = "HTTP/1.1 " + ss.str() + " " + statusMessage + "\n";
  responseHeader += serverVersion + "\n";

  responseHeader += "Date: ";
  responseHeader += HttpTime::getCurrentTime();
  if (msgBody.size() != 0) {
    responseHeader += "Last-Modified: ";
    responseHeader += HttpTime::fmtDate(timeAndDate);
  }

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

void HttpResponse::setDateAndTime(const time_t miliseconds) {
  timeAndDate = miliseconds;
}
