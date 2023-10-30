/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 20:36:09 by lfarias-          #+#    #+#             */
/*   Updated: 2023/10/28 22:04:21 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpParser.hpp"

#include <sstream>

namespace utils {
  std::string toLowerStr(std::string str);
}

void HttpParser::parseRequestLine(std::string *requestLine, 
                                         HttpRequest *request) {
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

  if (fields[1].size() > 1 && fields[1].at(fields[1].size() - 1) == '/') {
    fields[1].erase(fields[1].size() - 1, 1);
  }

  request->setResource(fields[1]);
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
}

bool HttpParser::parseHeaders(std::string *msg, HttpHeaders *headers) {
  bool emptyLineFound = false;
  std::istringstream msgStream(*msg);
  std::string line;

  while (std::getline(msgStream, line)) {
    if (line.size() > 0 && line[line.size() - 1] == '\r') {
      line.erase(line.size() - 1);
    }

    if (line.empty() && !emptyLineFound) {
      emptyLineFound = true;
      continue;
    } else if (line.empty() && emptyLineFound) {
      continue;
    } else if (!line.empty() && emptyLineFound) {
      /*if (request->getMethod() != "POST") {
        std::cout << "debug: bad request body" << std::endl;
        return false;
      }
      else
        break */;
      break;
    }

    size_t delim_pos = line.find(':');
    size_t ws_pos = line.find_first_of(" \t");
    size_t char_pos = line.find_first_not_of(" \t");

    if (delim_pos == std::string::npos)
      break;

    if (ws_pos != std::string::npos && \
        (ws_pos < char_pos || ws_pos != delim_pos + 1)) {
      return false;
    }

    std::string key = utils::toLowerStr(line.substr(0, delim_pos));
    std::string value;
    size_t val_start = line.find_first_not_of(" \t", delim_pos + 1);
    size_t val_end = line.find_last_not_of(" \t", delim_pos + 1);

    if (val_start != std::string::npos) {
      value = line.substr(val_start, val_end - val_start);
    } else {
      value = "";
    }

    headers->insert(std::make_pair(key, value));
  }

  return true;
}

bool HttpParser::parseProtocolVersion(const std::string &input, 
                                      int *mainVersion, int *subVersion) {
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

std::string utils::toLowerStr(std::string str) {
  std::string result;

  for (size_t i = 0; i < str.size(); i++) {
    result += static_cast<char>(std::tolower(str[i]));
  }

  return (result);
}

