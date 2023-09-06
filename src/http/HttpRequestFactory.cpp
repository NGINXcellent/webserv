/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:44:48 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/05 18:27:57 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/http/HttpRequestFactory.hpp"

#include <fstream> // io ops
#include <iostream> // cout
#include <map>
#include <sstream>
#include <vector>

#include "../../include/http/HttpParser.hpp"
#include "../../include/http/HttpStatus.hpp"
#include "../../include/io/FileSystem.hpp"

namespace post {
  void setupContentType(const std::string &msg, HttpRequest *request);
  void setupRequestBody(const std::string &msg, HttpRequest *request);
  void urlEncodedBodyType(const std::string &msg, HttpRequest *request);
}

namespace location {
  std::vector<std::string> splitPath(const std::string &path);
}

void HttpRequestFactory::setupHeader(HttpRequest *request, std::string &requestMsg) {
  HttpParser parser;
  size_t pos = requestMsg.find_first_of("\n", 0);

  if (pos == std::string::npos) {
    std::cout << "debug: no newline on requestline" << std::endl;
    return;
  }

  // extract and translate request line
  std::string reqLine = requestMsg.substr(0, pos);
  requestMsg.erase(0, pos + 1);
  parser.parseRequestLine(&reqLine, request);

  HttpHeaders headers;

  if (!parser.parseHeaders(&requestMsg, &headers)) {
    request->setProtocolName("");
  }

  request->setHost(getHeaderValue("host", &headers));
  request->setModifiedTimestampCheck( \
                                getHeaderValue("if-modified-since", &headers));
  request->setUnmodifiedSinceTimestamp( \
                                getHeaderValue("if-unmodified-since", &headers));
  request->setPostType(setupBodyContentType(request, headers));
  request->setContentLength(getHeaderValue("content-length", &headers));
}

void HttpRequestFactory::setupRequest(HttpRequest *req, std::string &requestMsg, \
                                      LocationList locations) { 
  findLocation(req, locations);

  // setup Body if POST
  if(req->getPostType() != None && checkMaxBodySize(req, locations)){
    post::setupRequestBody(requestMsg, req);
  }
}

HttpRequest *HttpRequestFactory::createFrom(std::string &requestMsg, \
                                            LocationList locations) {
  // shim
  (void)locations;
  HttpParser parser;
  HttpRequest *request = new HttpRequest();
  size_t pos = requestMsg.find_first_of("\n", 0);

  if (pos == std::string::npos) {
    std::cout << "debug: no newline on requestline" << std::endl;
    return request;
  }

  // extract and translate request line
  std::string reqLine = requestMsg.substr(0, pos);
  requestMsg.erase(0, pos + 1);
  parser.parseRequestLine(&reqLine, request);

  // extract the headers
  HttpHeaders headers;

  if (!parser.parseHeaders(&requestMsg, &headers)) {
    request->setProtocolName("");
  }

  //  setup body if POST

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

bool HttpRequestFactory::checkMaxBodySize(HttpRequest *request, \
                                          LocationList locations) {
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

PostType HttpRequestFactory::setupBodyContentType(HttpRequest *request, \
                                                     HttpHeaders &headers) {
  std::string postType = getHeaderValue("transfer-encoding", &headers);
  std::string boundary = "boundary=";

  if (postType == "chunked") {
    return Chunked;
  }

  postType = getHeaderValue("content-type", &headers);

  if (postType.find("application/x-www-form-urlencoded") != std::string::npos) {
    return UrlEncoded;
  }

  if (postType.find("multipart/form-data") != std::string::npos) {
    size_t boundaryPos = postType.find(boundary);

    if (boundaryPos != std::string::npos) {
      boundaryPos += strlen(boundary.c_str());
      size_t boundaryEndPos = postType.find("\r\n", boundaryPos);
      std::string boundary =
          postType.substr(boundaryPos, boundaryEndPos - boundaryPos);
      request->setBoundary(boundary);
      return Multipart;
    }
  }

  return None;
}

void MultipartBodyType(const std::string &msg, HttpRequest *request) {
  MultiPartMap bodyParts;
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
    size_t namePos = bodyPart.find("name=\"") + 6;
    size_t nameEndPos = bodyPart.find("\"", namePos);
    std::string name = bodyPart.substr(namePos, nameEndPos - namePos);

    size_t contentPos = bodyPart.find("\r\n\r\n") + 4;
    std::string content =
        bodyPart.substr(contentPos, bodyPart.size() - contentPos - 4);

    bodyParts[name] = content;
    startPos = endPos + boundary.length();
  }

  if (!bodyParts.empty()) {
    request->setMultipartMap(bodyParts);
  }
}

void chunkBodyType(const std::string &msg, HttpRequest *request) {
  size_t pos;
  size_t count = 1;
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

void post::setupRequestBody(const std::string &msg, HttpRequest *request) {
  PostType p_type = request->getPostType();
 
  switch (p_type) {
    case Chunked:
      chunkBodyType(msg, request);
      break;

    case Multipart:
      MultipartBodyType(msg, request);
      break;

    case UrlEncoded:
      urlEncodedBodyType(msg, request);
      break;
    
    case None:
      break;
   }

  return;
}

std::vector<std::string> location::splitPath(const std::string &path) {
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

void HttpRequestFactory::findLocation(HttpRequest *request, \
                                      LocationList locations) {
  std::string reqLine = request->getResource();

  if(reqLine.empty()) {
    return;
  }

  std::cout << "reqLine " << reqLine << std::endl;
  std::vector<std::string> reqTokens = location::splitPath(reqLine);
  int locationMatch = 0; // the more specific match is the one that wins
  std::string path;
  std::string indexPath;
  std::string token;

  for (size_t i = 0; i < locations.size(); ++i) {
    std::vector<std::string> locTokens = location::splitPath(locations[i].location);
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
 
  return;
}

std::string HttpRequestFactory::getHeaderValue(std::string headerName, \
                                               HttpHeaders* headers) {
  // shim
  size_t char_pos = (*headers)[headerName].find_first_not_of(" \t");

  if (char_pos != std::string::npos) {
    return (*headers)[headerName].substr(char_pos);
  }

  return ("");
}

void post::urlEncodedBodyType(const std::string &msg, HttpRequest *request) {
  std::string ret;
  size_t pos;

  if ((pos = msg.find("\r\n\r\n")) == std::string::npos) {
    return;
  }
  pos += 4;

  ret = msg.substr(pos, request->getContentLength());

  request->setRequestBody(ret);
}
