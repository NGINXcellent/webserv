/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:44:48 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/02 20:24:43 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequestFactory.hpp"
#include "../../include/http/HttpStatus.hpp"
#include "../../include/io/FileSystem.hpp"
#include "../../include/http/Server.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

namespace parsing {
  void parseRequestLine(std::string *requestLine, HttpRequest *request, \
                        std::vector<struct s_locationConfig> locations);
  bool parseHeaders(std::string *msg, HttpRequest *request);
  bool parseProtocolVersion(const std::string &input, int *mainVer, int *subVer);
  void setupContentType(const std::string &msg, HttpRequest *request);
  void setupRequestBody(const std::string &msg, HttpRequest *request);
  std::string getHeaderValue(std::string headerName, std::map<std::string, std::string> headers);
  std::string toLowerStr(std::string str);
  std::vector<std::string> splitPath(const std::string &path);
}

HttpRequest *HttpRequestFactory::createFrom(std::string &requestMsg, \
                                    std::vector<s_locationConfig> locations) {
  // shim
  using namespace parsing;
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

  //  setup body if POST
  if(request->getPostType() != "NONE" && checkMaxBodySize(request, locations)){
    setupRequestBody(requestMsg, request);
  }

  return (request);
}

HttpStatusCode HttpRequestFactory::check(HttpRequest *request) {
  int mainVersion = request->getProtocolMainVersion();
  int minorVersion = request->getProtocolSubVersion();
  int version = mainVersion * 10 + minorVersion;

  if (request->getProtocolName() != "HTTP" ||
      (mainVersion < 1 || minorVersion < 0)) {
    return (Bad_Request);
  }

  if (!(version == 10 || version == 11)) return (Http_Ver_Unsupported);

  if (version == 11 && request->getHost().size() == 0) return (Bad_Request);

  std::string method = request->getMethod();

  for (size_t i = 0; i < method.size(); i++) {
    if (!std::isupper(method[i])) {
      return (Bad_Request);
    }
  }

  return (Ready);
}

bool HttpRequestFactory::checkMaxBodySize(HttpRequest *request, std::vector<s_locationConfig> locations) {
  s_locationConfig tmp;
  bool hasLocationBodySize = false;
  
  for(size_t i = 0; i < locations.size(); ++i) {
    if(locations[i].location == request->getBaseLocation()) {
      tmp = locations[i];
      hasLocationBodySize = true;
      break;
    }
  }
 
  // remake logic
  if(hasLocationBodySize && request->getContentLength() > tmp.loc_max_body_size){
    request->setRedirectionCode(Payload_Too_Large);
    return false;
  }
  
  return true;
}

std::string setupBodyContentType(HttpRequest *request,
                                 std::map<std::string, std::string> &headers) {
  using namespace parsing;
  std::string postType = getHeaderValue("transfer-encoding", headers);
  if (postType == "chunked") {
    return "CHUNK";
  }

  postType = getHeaderValue("content-type", headers);
  if (postType.find("multipart/form-data") != std::string::npos) {
    size_t boundaryPos = postType.find("boundary=");

    if (boundaryPos != std::string::npos) {
      boundaryPos += 9;  // Tamanho da string "boundary="
      size_t boundaryEndPos = postType.find("\r\n", boundaryPos);
      std::string boundary =
          postType.substr(boundaryPos, boundaryEndPos - boundaryPos);
      request->setBoundary(boundary);
      return "MULTIPART";
    }
  }
  return "NONE";
}

void MultipartBodyType(const std::string &msg, HttpRequest *request) {
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

    // Extract name and content from bodyPart
    s_multipartStruct multipartStruct;
    size_t namePos = bodyPart.find("name=\"") + 6;
    size_t nameEndPos = bodyPart.find("\"", namePos);
    multipartStruct.name = bodyPart.substr(namePos, nameEndPos - namePos);

    size_t contentPos = bodyPart.find("\r\n\r\n") + 4;
    multipartStruct.content =
        bodyPart.substr(contentPos, bodyPart.size() - contentPos - 4);

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

  if (!ret.empty()) {
    request->setRequestBody(ret);
  }
}

void setupRequestBody(const std::string &msg, HttpRequest *request) {
  if (request->getPostType() == "CHUNK") {
    chunkBodyType(msg, request);
  }
  
  if (request->getPostType() == "MULTIPART") {
    MultipartBodyType(msg, request);
    return;
  }
  
  return;
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
  int locationMatch = 0; // the more specific match is the one that wins
  std::string path;
  std::string indexPath;
  std::string token;

  for (size_t i = 0; i < locations.size(); ++i) {
    std::vector<std::string> locTokens = splitPath(locations[i].location);
    int tokenMatches = 0;
   
    for (size_t j = 0; j < locTokens.size(); j++) {
      std::cout << "reqTokens >>>> " << reqTokens[j] << std::endl;
      std::cout << "locTokens >>>> " << locTokens[j] << std::endl;
      if (reqTokens[j] == locTokens[j]) {
        tokenMatches++; 
      } else {
        break;
      }
    }

    if (tokenMatches < locationMatch) {
      continue;
    } 
    
    locationMatch = tokenMatches;

    if(!locations[i].redirect.second.empty()) {
      request->setRedirectionCode(locations[i].redirect.first);
      request->setRedirectionPath(locations[i].redirect.second);
      continue; // go to next location config
    } else {
      request->setRedirectionCode(0);
      request->setRedirectionPath("");
    }
  
    std::string fullpath = locations[i].root + reqLine; 

    std::cout << "index: " << locations[i].index << std::endl;
    if (FileSystem::isDirectory(fullpath.c_str()) && !locations[i].index.empty()) {
      indexPath = fullpath + '/' + locations[i].index;
    } else {
      indexPath = "";
    }

    request->setRoot(locations[i].root);
    request->setDirListActive(locations[i].autoindex);
    request->setAllowedMethods(locations[i].allowed_method);
    request->setIndexPath(indexPath);
  }
 
  request->setResource(reqLine);
  return;
}

void parseRequestLine(std::string *requestLine, HttpRequest *request, \
                      std::vector<struct s_locationConfig> locations) {
  using namespace parsing;
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
  using namespace parsing;
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
  request->setPostType(setupBodyContentType(request, headers));
  request->setContentLength(getHeaderValue("content-length", headers));

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

