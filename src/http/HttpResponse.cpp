/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:50:49 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/01 21:13:00 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpResponse.hpp"

#include <sstream>

#define SERVER_VERSION "0.1\n"

HttpResponse::HttpResponse(void) {
  statusCode = 200;
  statusMessage = "OK";
  contentLength = 0;
  serverVersion = "Server: webserv/";
  serverVersion += SERVER_VERSION;
}

HttpResponse::~HttpResponse(void) {}

std::string   HttpResponse::getHeaders(void) {
  std::stringstream ss;
  ss << statusCode;
  std::string responseHeader = "HTTP/1.1 " + ss.str() + " " + statusMessage + "\n";
  responseHeader += serverVersion;
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

void HttpResponse::setMsgBody(const std::vector<char>& data) {
  msgBody = data;
}

void HttpResponse::setContentType(const std::string &mime) {
  contentType = mime;
}
