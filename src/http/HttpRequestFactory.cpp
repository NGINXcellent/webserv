/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:44:48 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/04 11:09:43 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequestFactory.hpp"
#include <iostream>
#include <map>
#include <sstream>

void parseRequestLine(std::string& msg, HttpRequest *request);
void parseHeaders(std::string &msg, HttpRequest *request);

std::string toLowerStr(std::string str);

HttpRequest *HttpRequestFactory::createFrom(char *requestMsg) {
  std::string msg(requestMsg);
  HttpRequest *request = new HttpRequest();

  parseRequestLine(msg, request);
  parseHeaders(msg, request);
  return (request);
}

void parseRequestLine(std::string& msg, HttpRequest *request) {
  size_t pos = msg.find(' ');
  request->setMethod(msg.substr(0, pos));

  if (pos != std::string::npos)
    msg.erase(0, pos + 1);
  pos = msg.find(' ');

  std::string resource = "./static_pages" + msg.substr(0, pos);
  request->setResource(resource);
  msg.erase(0, pos + 1);

  int lf_pos = msg.find('\n');
  pos = msg.find('/');
  std::string protoName = msg.substr(0, pos);
  std::string versionString = msg.substr(pos + 1);
  int mainVersion, subVersion;
  std::stringstream versionStream(versionString);
  char dot = '.';
  versionStream >> mainVersion >> dot >> subVersion;
  request->setProtocolName(protoName);
  request->setProtocolVersion(mainVersion, subVersion);

  msg.erase(0, lf_pos + 1);
}

void parseHeaders(std::string &msg, HttpRequest *request) {
  std::map<std::string, std::string> headers;

  while (msg.size() != 0) {
    int pos = msg.find(':');
    std::string key = toLowerStr(msg.substr(0, pos));
    msg.erase(0, pos + 1);

    pos = msg.find('\n');
    std::string value = msg.substr(0, pos);
    msg.erase(0, pos + 1);

    headers.insert(std::make_pair(key, value));
  }

  request->setHost(headers["host"]);
  //  std::cout << "'" << request->getHost() << "'" << std::endl;
  //  std::cout << "'" << request->getProtocolVersion() << "'" << std::endl;

  // todo: get the If-Modified-Since and If-Unmodified-Since
}

std::string toLowerStr(std::string str) {
  std::string result;

  for (size_t i = 0; i < str.size(); i++) {
      result += static_cast<char>(std::tolower(str[i]));
  }

  return result;
}
