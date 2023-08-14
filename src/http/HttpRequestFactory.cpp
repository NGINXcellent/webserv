/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:44:48 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/14 15:47:29 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequestFactory.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <sys/stat.h>

#include "../../include/http/Server.hpp"

void parseRequestLine(std::string *msg, HttpRequest *request, std::string location);
void parseHeaders(std::string *msg, HttpRequest *request);
bool parseProtocolVersion(const std::string &input, int *mainVer, int *subVer);
std::string createLocation(char *buffer, std::vector<s_locationConfig> locations, HttpRequest *request);
std::string getHeaderValue(std::string headerName, std::map<std::string, std::string> headers);
std::string toLowerStr(std::string str);

HttpRequest *HttpRequestFactory::createFrom(char *requestMsg, \
                                    std::vector<s_locationConfig> locations) {
  // shim
  HttpRequest *request = new HttpRequest();
  std::string location = createLocation(requestMsg, locations, request);
  std::string msg(requestMsg);
  size_t pos = msg.find_first_of('\n');

  if (pos == std::string::npos) {
    return request;
  }

  std::string reqLine = msg.substr(0, pos);
  msg.erase(0, pos + 1);
  parseRequestLine(&reqLine, request, location);
  parseHeaders(&msg, request);
  return (request);
}

bool tokensValidator(std::vector<s_locationConfig> locations,
                     std::vector<std::string> tokens) {
  for (size_t i = 0; i < locations.size(); ++i) {
    if (locations[i].location == tokens[0]) return true;
  }
  return false;
}

bool isDirectory(const char* path) {
    struct stat fileInfo;
    if (stat(path, &fileInfo) != 0) {
        return false; // Erro ao obter informações do arquivo
    }
    return S_ISDIR(fileInfo.st_mode);
}

std::string createLocation(char *buffer,
                           std::vector<s_locationConfig> locations,
                           HttpRequest *request) {
    (void)request;
    std::istringstream streaming(buffer);
    std::string line;
    streaming >> line >> line;
    std::vector<std::string> tokens;

    std::istringstream iss(line);
    std::string token;

    if (line == "/") {
        tokens.push_back("/");
    } else {
        while (std::getline(iss, token, '/')) {
          if (!token.empty()) {
            token = '/' + token;
            tokens.push_back(token);
          }
        }
    }

    if (!tokensValidator(locations, tokens))
      tokens.insert(tokens.begin(), "/");

    for (size_t i = 0; i < locations.size(); ++i) {
        if (locations[i].location == tokens[0]) {
          std::string ret;
          if (locations[i].root.empty())
            ret = '.' + line;
          else {
            ret += "." + locations[i].root;
            for (size_t j = 1; j < tokens.size(); ++j) {
              ret += tokens[j];
            }
          }
          if (isDirectory(ret.c_str()) && !locations[i].index.empty()) {
            if (ret != "./")
              ret += '/' + locations[i].index;
            else
              ret += locations[i].index;
          }
          request->setAllowedMethods(locations[i].allowed_method);
          return ret;
        }
    }
    return "";
}

void parseRequestLine(std::string *requestLine, HttpRequest *request,
                      std::string location) {
  std::vector<std::string> fields;

  while (requestLine->size() != 0) {
    size_t begin = requestLine->find_first_not_of(" \t");
    size_t end = requestLine->find_first_of(" \t", begin);

    if (begin == std::string::npos) {
      break;
    }
    fields.push_back(requestLine->substr(begin, end - begin));
    requestLine->erase(0, end);
  }

  if (fields.size() != 3) {
    return;
  }

  request->setMethod(fields[0]);
  request->setResource(location);

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

void parseHeaders(std::string *msg, HttpRequest *request) {
  std::map<std::string, std::string> headers;

  while (msg->size() != 0) {
    size_t pos = msg->find(':');
    size_t ws_pos = msg->find_first_of(" \t");
    size_t char_pos = msg->find_first_not_of(" \t");

    if (pos == std::string::npos) {
      break;
    }

    if (ws_pos < char_pos) {
      request->setProtocolName("");  // invalidates the request
      return;
    }

    std::string key = toLowerStr(msg->substr(0, pos));
    msg->erase(0, pos + 1);
    pos = msg->find('\n');

    if (pos == std::string::npos) {
      request->setProtocolName("");  // invalidates the request
      return;
    }

    std::string value = msg->substr(0, pos);
    msg->erase(0, pos + 1);
    headers.insert(std::make_pair(key, value));
  }

  request->setHost(getHeaderValue("host", headers));
  request->setModifiedTimestampCheck( \
                                getHeaderValue("if-modified-since", headers));
  request->setUnmodifiedSinceTimestamp( \
                                getHeaderValue("if-unmodified-since", headers));
}

bool parseProtocolVersion(const std::string &input, int *mainVersion,
                          int *subVersion) {
  size_t dot_pos = input.find('.');

  if (input.empty() || dot_pos == 0 || dot_pos == input.size() - 1 ||
      dot_pos == std::string::npos) {
      return (false);
  }

  std::istringstream iss(input);
  int majorVersion = 0;
  int minorVersion = 0;
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

std::string getHeaderValue(std::string headerName, \
                           std::map<std::string, std::string> headers) {
  // shim
  size_t char_pos = headers[headerName].find_first_not_of(" \t");

  if (char_pos != std::string::npos) {
    return headers[headerName].substr(char_pos);
  }

  return ("");
}

std::string toLowerStr(std::string str) {
  std::string result;

  for (size_t i = 0; i < str.size(); i++) {
    result += static_cast<char>(std::tolower(str[i]));
  }

  return (result);
}

int HttpRequestFactory::check(HttpRequest *request) {
  int mainVersion = request->getProtocolMainVersion();
  int minorVersion = request->getProtocolSubVersion();
  int version = mainVersion * 10 + minorVersion;

  if (request->getProtocolName() != "HTTP" ||
      (mainVersion < 1 || minorVersion < 0))
    return (400);
  if (!(version == 10 || version == 11)) return (505);
  if (version == 11 && request->getHost().size() == 0) return (400);

  std::string method = request->getMethod();
  for (size_t i = 0; i < method.size(); i++) {
    if (!std::isupper(method[i])) return (400);
  }

  return (0);
}
