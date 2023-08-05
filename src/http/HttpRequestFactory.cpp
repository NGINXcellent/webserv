/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:44:48 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/05 19:11:28 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequestFactory.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <sstream>

void parseRequestLine(std::string& msg, HttpRequest *request);
void parseHeaders(std::string &msg, HttpRequest *request);
bool isValidNumberPattern(const std::string& input);
bool parseProtocolVersion(const std::string& input, int* mainVersion, int* subVersion);
int convertStrToInt(std::string protocolVersion);

std::string toLowerStr(std::string str);

HttpRequest *HttpRequestFactory::createFrom(char *requestMsg) {
  HttpRequest *request = new HttpRequest();

  std::string msg(requestMsg);
  size_t pos = msg.find_first_of('\n');

  if (pos == std::string::npos) {
    return request;
  }

  std::string reqLine = msg.substr(0, pos);
  msg.erase(0, pos + 1);

  parseRequestLine(reqLine, request);
  parseHeaders(msg, request);
  return (request);
}

void parseRequestLine(std::string& requestLine, HttpRequest *request) {
  std::vector<std::string> fields;

  while (requestLine.size() != 0) {
    size_t begin = requestLine.find_first_not_of(" \t");
    size_t end = requestLine.find_first_of(" \t", begin);

    if (begin == std::string::npos) {
      break;
    }

    fields.push_back(requestLine.substr(begin, end - begin));
    requestLine.erase(0, end);
  }

  if (fields.size() != 3) {
    std::cout << "give me chocolate" << std::endl;
    return;
  }

  request->setMethod(fields[0]);

  std::string resource = "./static_pages" + fields[1];
  request->setResource(resource);

  size_t pos = fields[2].find('/');

  if (pos == std::string::npos) {
    return;
  }

  std::string protocol = fields[2].substr(0, pos);
  request->setProtocolName(protocol);
  std::string protocolVersion = fields[2].substr(pos + 1);

  int mainVersion;
  int minorVersion;

  if (!(parseProtocolVersion(protocolVersion, &mainVersion, &minorVersion)))
    return;

  request->setProtocolVersion(mainVersion, minorVersion);
  // std::cout << msg << std::endl;
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
  // std::cout << "'" << request->getHost() << "'" << std::endl;
  // std::cout << "'" << request->getProtocolVersion() << "'" << std::endl;

  // todo: get the If-Modified-Since and If-Unmodified-Since
}

bool parseProtocolVersion(const std::string& input, int* mainVersion, int* subVersion) {
  size_t dot_pos = input.find('.');

  if (input.empty() || dot_pos == 0 || dot_pos == input.size() - 1 || \
      dot_pos == std::string::npos) {
      return false;
  }

  /*if ((std::isdigit(input[dot_pos - 1]) && std::isdigit(input[dot_pos + 1])))
    return true;
  else
    return false;*/
  std::istringstream iss(input);
  int majorVersion = -1;
  int minorVersion = -1;
  char dot = '.';

  if (iss >> majorVersion >> dot >> minorVersion) {
    char leftover;
    if (iss >> leftover) {
      return (false);
    }
  } else {
    return (false);
  }
  *mainVersion = majorVersion;
  *subVersion = minorVersion;
  return (true);
}

std::string toLowerStr(std::string str) {
  std::string result;

  for (size_t i = 0; i < str.size(); i++) {
      result += static_cast<char>(std::tolower(str[i]));
  }

  return result;
}

int HttpRequestFactory::check(HttpRequest *request) {
  int mainVersion = request->getProtocolMainVersion();
  int minorVersion = request->getProtocolSubVersion();
  int version = mainVersion * 10 + minorVersion;

  if (request->getProtocolName() != "HTTP" || \
    (mainVersion < 1 || minorVersion < 0))
    return (400);
  if (!(version == 10 || version == 11))
    return (505);
  if (version == 11 && request->getHost().size() == 0)
    return (400);

  std::string method = request->getMethod();
  for (size_t i = 0; i < method.size(); i++) {
    if (!std::isupper(method[i]))
      return (400);
  }

  return (0);
}

int convertStrToInt(std::string protocolVersion) {
  std::istringstream iss(protocolVersion);
  int result;

  if (iss >> result) {
    char leftover;
    if (iss >> leftover) {
      return (-1);
    }
  } else {
    return (-1);
  }

  return (result);
}
