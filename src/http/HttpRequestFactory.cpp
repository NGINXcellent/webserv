/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestFactory.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 21:44:48 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/21 08:25:33 by dvargas          ###   ########.fr       */
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
bool parseHeaders(std::string *msg, HttpRequest *request);
bool parseProtocolVersion(const std::string &input, int *mainVer, int *subVer);
std::string createLocation(char *buffer, std::vector<s_locationConfig> locations, HttpRequest *request);
std::string getHeaderValue(std::string headerName, std::map<std::string, std::string> headers);
std::string toLowerStr(std::string str);

void setupContentType(std::string msg, HttpRequest *request) {
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
      return;
    }
  }
  request->setPostType("NONE");
}


void setupRequestBody(std::string msg, HttpRequest *request) {
      size_t pos, count = 1;
    std::string ret;
    // Encontra a posição após os headers da requisição
    if ((pos = msg.find("\r\n\r\n")) == std::string::npos) // Verifica se há pelo menos os headers
        return;
    pos += 2;
    while (count) {
        pos += 2; // Ajusta a posição para pular os caracteres de quebra de linha
        
        // Extrai o tamanho do chunk (em hexadecimal) e adiciona ao _c->clen
        count = strtol(&msg[pos], NULL, 16);
        // content-len += count;
        
        // Encontra a posição do próximo caractere de nova linha (\n)
        pos = msg.find_first_of("\n", pos) + 1;
        
        // Extrai o corpo do chunk e o armazena em _body
        ret.append(msg, pos, count);
        
        pos += count; // Move a posição para o próximo chunk
    }
    // MAX BODY SIZE CHECK POSSIVELMENTE VEM AQUI
    if (!ret.empty())
        request->setRequestBody(ret);
}

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

  setupContentType(msg, request);
  setupRequestBody(msg, request);

//  TODO TALVEZ TENHA QUE FAZXER UM LOCATION ESPECIRFICO PARA ESSA SITUACAO AQUI
  request->setlocationTest(location);


  // extract request line
  std::string reqLine = msg.substr(0, pos);
  msg.erase(0, pos + 1);
  parseRequestLine(&reqLine, request, location);

  // extract the headers
  if (!parseHeaders(&msg, request))
    request->setProtocolName("");
  return (request);
}

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
    if(line.empty())
      return "";
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

    if (!tokensValidator(locations, request, tokens))
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
          std::cout << "ret de location " << ret << std::endl;
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
      if (request->getMethod() != "POST")
        return false;
      else
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

    if (val_start != std::string::npos)
      value = line.substr(val_start, val_end - val_start);
    else
      value = "";

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
