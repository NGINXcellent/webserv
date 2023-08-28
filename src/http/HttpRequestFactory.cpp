/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:44:48 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/28 14:20:53 by lfarias-         ###   ########.fr       */
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

void parseRequestLine(std::string *requestLine, HttpRequest *request, \
                      std::vector<struct s_locationConfig> locations);
bool parseHeaders(std::string *msg, HttpRequest *request);
bool parseProtocolVersion(const std::string &input, int *mainVer, int *subVer);
void setupContentType(const std::string &msg, HttpRequest *request);
void setupRequestBody(const std::string &msg, HttpRequest *request);
std::string getHeaderValue(std::string headerName, std::map<std::string, std::string> headers);
std::string toLowerStr(std::string str);
std::vector<std::string> splitPath(const std::string &path);

HttpRequest *HttpRequestFactory::createFrom(std::string &requestMsg, \
                                    std::vector<s_locationConfig> locations) {
  // shim
  HttpRequest *request = new HttpRequest();
  size_t pos = requestMsg.find_first_of("\n", 0);

  if (pos == std::string::npos) {
    std::cout << "debug: no newline on requestline" << std::endl;
    return request;
  }

  // extract request line
  std::string reqLine = requestMsg.substr(0, pos);
  requestMsg.erase(0, pos + 1);
  parseRequestLine(&reqLine, request, locations);

  // extract the headers
  if (!parseHeaders(&requestMsg, request)) {
    request->setProtocolName("");
  }

  setupContentType(requestMsg, request);
  setupRequestBody(requestMsg, request);

  return (request);
}

void setupContentType(const std::string &msg, HttpRequest *request) {
  size_t pos = msg.find("Transfer-Encoding:");

  if (pos != std::string::npos) {
    if (msg.find("chunked", pos) != std::string::npos) {
      request->setPostType("CHUNK");
      return;
    }
  }

  pos = msg.find("Content-Type:");

  if (pos != std::string::npos) {
    if (msg.find("multipart/form-data", pos) != std::string::npos) {
      request->setPostType("MULTIPART");
      // Pegar o boundary do multipart
      size_t boundaryPos = msg.find("boundary=", pos);
      if (boundaryPos != std::string::npos) {
        boundaryPos += 9; // Tamanho da string "boundary="
        size_t boundaryEndPos = msg.find("\r\n", boundaryPos);
        std::string boundary = msg.substr(boundaryPos, boundaryEndPos - boundaryPos);
        request->setBoundary(boundary);
      }
      return;
    }
  }

  request->setPostType("NONE");
}

void MultipartBodyType(const std::string &msg, HttpRequest *request) {
  //P("DENTRO DO EXTRATOR");
    std::vector<s_multipartStruct> bodyParts;
    std::string boundary = request->getBoundary();
    size_t startPos = msg.find(boundary);

    if (startPos == std::string::npos) {
        return;
    }

    startPos += boundary.length();

    while (startPos != std::string::npos) {
        size_t endPos = msg.find(boundary, startPos);

        if (endPos == std::string::npos) {
            break;
        }

        std::string bodyPart = msg.substr(startPos, endPos - startPos);

        // Extract name, and content from bodyPart
        s_multipartStruct multipartStruct;
        size_t namePos = bodyPart.find("name=\"") + 6;
        size_t nameEndPos = bodyPart.find("\"", namePos);
        multipartStruct.name = bodyPart.substr(namePos, nameEndPos - namePos);

        size_t contentPos = bodyPart.find("\r\n\r\n") + 4;
        multipartStruct.content = bodyPart.substr(contentPos, bodyPart.size() - contentPos - 4);

        bodyParts.push_back(multipartStruct);
        startPos = endPos + boundary.length();
    }

    if (!bodyParts.empty()) {
        request->setMultipartStruct(bodyParts);
    }
}

void chunkBodyType (const std::string &msg, HttpRequest *request) {
    size_t pos, count = 1;
    std::string ret;

    if ((pos = msg.find("\r\n\r\n")) == std::string::npos) {
        return;
    }

    pos += 2;

    while (count) {
        pos += 2;
        count = strtol(&msg[pos], NULL, 16);
        pos = msg.find_first_of("\n", pos) + 1;
        ret.append(msg, pos, count);
        pos += count;
    }
    // MAX BODY SIZE CHECK POSSIVELMENTE VEM AQUI
    if (!ret.empty())
        request->setRequestBody(ret);
}

void setupRequestBody(const std::string &msg, HttpRequest *request) {
    // P("Início");
    std::cout << msg << std::endl;
    if(request->getPostType() == "NONE") {
        return;
    }

    if(request->getPostType() == "MULTIPART") {
        // P("if MULTIPART");
        MultipartBodyType(msg, request);
        return;
    }

    chunkBodyType(msg, request);
}
//   if(request->getPostType() == "NONE") {
//     return;
//   }
//   if(request->getPostType() == "MULTIPART") {
//     std::cout << "entrou aqui" << std::endl;
//     // std::cout << msg << std::endl;
//     MultipartBodyType(msg, request);
//     return;
//   }
//   chunkBodyType(msg, request);
// }

// this function verify if the tokens are in the right order and deals with redirection
// if fails we are dealing whith location "/"
bool tokensValidator(std::vector<s_locationConfig> locations, HttpRequest *request,
                     std::vector<std::string> &tokens) {
  for (size_t i = 0; i < locations.size(); ++i) {
    if (locations[i].location == tokens[0]){
      if(!locations[i].redirect.second.empty()) {
        tokens[0] = locations[i].redirect.second;
        request->setResponseStatusCode(locations[i].redirect.first);
      }
      return true;
    }
  }
  return false;
}

bool isDirectory(const char* path) {
    struct stat fileInfo;

    if (stat(path, &fileInfo) != 0) {
        return false;
    }

    return S_ISDIR(fileInfo.st_mode);
}

std::vector<std::string> splitPath(const std::string &path) {
  std::vector<std::string> tokens;
  std::istringstream iss(path);
  std::string token;

  if (path == "/") {
      tokens.push_back("/");
  } else {
      while (std::getline(iss, token, '/')) {
        if (!token.empty()) {
          token = token;
          tokens.push_back(token);
        }
      }
  }

  return (tokens);
}

void HttpRequestFactory::findLocation(const std::string &reqLine,
                           std::vector<s_locationConfig> locations,
                           HttpRequest *request) {
  if(reqLine.empty()) {
    return;
  }

  std::cout << "reqLine " << reqLine << std::endl;
  std::vector<std::string> reqTokens = splitPath(reqLine);

  //if (!tokensValidator(locations, request, reqTokens))
    //reqTokens.insert(reqTokens.begin(), "/");

  int locationMatch = 0; // the more specific match is the one that wins
  std::string path;
  std::string indexedPath;
  std::string token;

  for (size_t i = 0; i < locations.size(); ++i) {
    std::vector<std::string> locTokens = splitPath(locations[i].location);
    int tokenMatches = 0;
   
    for (size_t j = 0; j < locTokens.size(); j++) {
      // std::cout << "reqTokens >>>> " << reqTokens[j] << std::endl;
      // std::cout << "locTokens >>>> " << locTokens[j] << std::endl;
      if (reqTokens[j] == locTokens[j]) {
        tokenMatches++; 
      } else {
        break;
      }
    }

    if (tokenMatches > locationMatch) {
      locationMatch = tokenMatches;
      request->setRoot(locations[i].root);
      path = locations[i].root + reqLine;

      if (isDirectory(path.c_str()) && !locations[i].index.empty()) {
        indexedPath = path + '/' + locations[i].index;
      } else {
        indexedPath = "";
      }

      request->setDirListActive(locations[i].autoindex);
      request->setAllowedMethods(locations[i].allowed_method);
    }
  }
 
  // path is the junction of root + reqLine
  // example: root = /www/html/ , reqLine = /lfarias
  // result: /www/html/lfarias
  std::cout << "REQ PATH: " << path << std::endl;
  std::cout << "INDEXED PATH: " << indexedPath << std::endl;
  request->setResource(reqLine);
  request->setLocation(indexedPath);
  return;
}

void parseRequestLine(std::string *requestLine, HttpRequest *request, \
                      std::vector<struct s_locationConfig> locations) {
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

  HttpRequestFactory::findLocation(fields[1], locations, request);
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

bool parseHeaders(std::string *msg, HttpRequest *request) {
  std::map<std::string, std::string> headers;
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
      // std::cout << "debug: line starts with white space" << std::endl;
      return false;
    }

    std::string key = toLowerStr(line.substr(0, delim_pos));
    std::string value;
    size_t val_start = line.find_first_not_of(" \t", delim_pos + 1);
    size_t val_end = line.find_last_not_of(" \t", delim_pos + 1);

    if (val_start != std::string::npos) {
      value = line.substr(val_start, val_end - val_start);
    } else {
      value = "";
    }

    headers.insert(std::make_pair(key, value));
  }

  request->setHost(getHeaderValue("host", headers));
  request->setModifiedTimestampCheck( \
                                getHeaderValue("if-modified-since", headers));
  request->setUnmodifiedSinceTimestamp( \
                                getHeaderValue("if-unmodified-since", headers));

  return true;
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
      (mainVersion < 1 || minorVersion < 0)) {
    return (400);
  }

  if (!(version == 10 || version == 11)) return (505);

  if (version == 11 && request->getHost().size() == 0) return (400);

  std::string method = request->getMethod();

  for (size_t i = 0; i < method.size(); i++) {
    if (!std::isupper(method[i])) {
      return (400);
    }
  }

  return (0);
}
