/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 20:50:49 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/28 16:52:02 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpResponse.hpp"

#include <sstream>

HttpResponse::HttpResponse(void) {
  statusCode = 200;
  statusMessage = "OK";
  contentType = "text/plain";
  contentLength = 0;
}

HttpResponse::~HttpResponse(void) {}

std::string   HttpResponse::getHeaders(void) {
  std::stringstream ss;
  ss << statusCode;
  std::string responseHeader = "HTTP/1.1 " + ss.str() + " " + statusMessage + "\n";
  responseHeader += "Content-Type: " + contentType + "\n";
  ss.clear();
  ss.str("");
  ss << 12;
  responseHeader += "Content-length: " + ss.str() + "\n";
  responseHeader += "Connection: close\n\n";
  responseHeader += "Hello World";
  return responseHeader;
}
