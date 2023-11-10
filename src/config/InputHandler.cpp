/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 12:05:52 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/10 04:35:58 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config/InputHandler.hpp"

#include <sstream>
#include <cstring>
#include <iostream>
#include <fstream>
#include <exception>
#include <cstdlib>
#include <limits>
#include <algorithm>  // sort

bool  InputHandler::check_args(int argc, const char **argv) {
  if ((argc - 1) == 1 && argv[1] != NULL) {
    size_t filenameLen = strlen(argv[1]);

    return (filenameLen > 5 && \
                strncmp(&argv[1][filenameLen - 5], ".conf", 5) == 0);
  }

  return ((argc - 1) == 0);
}

void InputHandler::addPort(std::ifstream &fileStream, std::vector<std::string> &string) {
    std::string word;
    fileStream >> word;

    if ((word.find_first_not_of("0123456789;")) != std::string::npos)
      throw std::runtime_error("bad port");

    int port(atoi(word.c_str()));

    if (port < 1024 || port > 49151)
      throw std::runtime_error("wrong port range");

    if (!(word.find_first_of(";") == word.size() - 1)) {
      throw std::runtime_error("no ; at the end of line");
    } else {
      word.resize(word.size() - 1);
      string.push_back(word);
    }
}

void InputHandler::addToSizeT(std::ifstream &fileStream, size_t &size) {
  std::string word;
  fileStream >> word;

  if (!(word.find_first_of(";") == word.size() - 1))
    throw std::runtime_error("no ; at the end of line");

  word.resize(word.size() -1);

  std::istringstream iss(word);
  char remainingChar;
  size_t value;

  if (!(iss >> value) || (iss >> remainingChar)) {
    throw std::runtime_error("Invalid size_t format");
  }
  size = value;
}

void InputHandler::addToString(std::ifstream &fileStream, \
                              std::string &string) {
  std::string word;
  fileStream >> word;

  if (!(word.find_first_of(";") == word.size() - 1))
    throw std::runtime_error("no ; at the end of line");

  word.resize(word.size() - 1);

  if (word.empty()) {
    throw std::runtime_error("empty entry");
    return;
  }

  string = word;
}

bool InputHandler::isAMethod(const std::string &word) {
  if (word == "GET" || word == "POST" || word == "DELETE" || word == "HEAD")
    return true;
  return false;
}

void InputHandler::addToVector(std::ifstream &fileStream, \
                               std::vector<std::string> &vec) {
  std::string word;
  bool hasdot = false;
  fileStream >> word;

  while (hasdot != true) {
    if (word.find_first_of(";") == word.size() - 1) {
      word.resize(word.size() - 1);
      if (isAMethod(word)) {
        vec.push_back(word);
        hasdot = true;
      } else {
        throw std::runtime_error("not a method");
      }
    } else {
      if (isAMethod(word)) {
        vec.push_back(word);
        fileStream >> word;
      } else {
        throw std::runtime_error("not a method");
      }
    }
  }
}

void InputHandler::addToPair(std::ifstream &fileStream, \
                            std::pair<int, std::string> &mapi) {
  std::string word;
  fileStream >> word;

  if ((word.find_first_not_of("0123456789")) != std::string::npos)
    throw std::runtime_error("bad number");

  int statusCode = std::atoi(word.c_str());
  fileStream >> word;

  if (!(word.find_first_of(";") == word.size() - 1))
    throw std::runtime_error("no ; at the end of line");

  word.resize(word.size() - 1);
  mapi.first = statusCode;
  mapi.second = word;
}

void InputHandler::extractExtension(std::ifstream &fileStream, \
                                   std::string &extension) {
  std::string word;
  fileStream >> word;
  if (!(word.find_first_of(";") == word.size() - 1))
    throw std::runtime_error("no ; at the end of line");

  word.resize(word.size() - 1);

  if(!word.empty() && word[0] != '.')
    throw std::runtime_error("extension should start with .");
  else
    extension = word;
}

void InputHandler::addToMap(std::ifstream &fileStream, \
                            std::map<int, std::string> &mapi) {
  std::string word;
  fileStream >> word;

  if ((word.find_first_not_of("0123456789")) != std::string::npos)
    throw std::runtime_error("bad number");

  int statusCode(atoi(word.c_str()));
  fileStream >> word;

  if (!(word.find_first_of(";") == word.size() - 1))
    throw std::runtime_error("no ; in end of line");

  word.resize(word.size() - 1);
  mapi.insert(std::make_pair(statusCode, word));
}

std::string locationCheck(std::string word) {
  if (word[0] != '/')
    throw std::runtime_error("location should start with /");
  return word;
}

void redirectCheck(s_locationConfig newLocation) {
    if (!newLocation.redirect.second.empty()) {
        if (!newLocation.root.empty() ||
            !newLocation.index.empty() ||
            newLocation.loc_max_body_size != SIZE_T_MAX ||
            !newLocation.allowed_method.empty() ||
            !newLocation.cgi_path.empty() ||
            !newLocation.cgi_extension.empty()) {
            throw std::runtime_error("return is not the only entrance");
        }
    }
}

void cgiCheck(s_locationConfig &newLocation) {
    if (newLocation.cgi_path.empty() && newLocation.cgi_extension.empty()) {
        return;
    }

    if (newLocation.cgi_path.empty() || newLocation.cgi_extension.empty()) {
        throw std::runtime_error("Both cgi_path and cgi_extension should be set or unset together");
    }

    newLocation.cgi_php = true;
}

void InputHandler::addToBool(std::ifstream &fileStream, \
                            bool &toBool) {
  std::string word;
  fileStream >> word;
  if (!(word.find_first_of(";") == word.size() - 1))
    throw std::runtime_error("no ; in end of line");
  if (word == "on;") {
      toBool = true;
  } else if (word == "off;") {
      toBool = false;
  } else {
      throw std::runtime_error("Wrong input, must be on or off");
  }
}

void InputHandler::addLocation(std::ifstream &fileStream, \
                               s_locationConfig &newLocation) {
  std::string word;
  fileStream >> word;
  newLocation.location = locationCheck(word);
  newLocation.loc_max_body_size = SIZE_T_MAX;
  fileStream >> word;

  if (word != "{")
    throw std::runtime_error("bracket error inside location");

  bool bracket = true;

  for (fileStream >> word; word != "}" && bracket; fileStream >> word) {
    if (word == "root") {
      if (!newLocation.root.empty())
        throw std::runtime_error("duplicate root inside location");
      addToString(fileStream, newLocation.root);

    } else if (word == "autoindex") {
      addToBool(fileStream, newLocation.autoindex);

    } else if (word == "index") {
      if (!newLocation.index.empty())
        throw std::runtime_error("duplicate index inside location");
      addToString(fileStream, newLocation.index);

    } else if (word == "cgi_path") {
      if (!newLocation.cgi_path.empty())
        throw std::runtime_error("duplicate cgi_path inside location");
      addToString(fileStream, newLocation.cgi_path);

    } else if (word == "cgi_extension") {
      if (!newLocation.cgi_extension.empty())
        throw std::runtime_error("duplicate cgi_extension inside location");
      extractExtension(fileStream, newLocation.cgi_extension);

    } else if (word == "max_body_size") {
      if (!newLocation.loc_max_body_size != 0)
        throw std::runtime_error("duplicate loc_max_body_size inside location");
      addToSizeT(fileStream, newLocation.loc_max_body_size);

    } else if (word == "return") {  // redirect
      addToPair(fileStream, newLocation.redirect);

    } else if (word == "allowed_method") {
      addToVector(fileStream, newLocation.allowed_method);

    } else if (word == "}") {
      bracket = false;
    } else {
      throw std::runtime_error("Error in location");
    }
  }
  cgiCheck(newLocation);
  redirectCheck(newLocation);
}

void InputHandler::serverNameAdd(std::ifstream &fileStream, \
                                std::vector<std::string> &servers) {
  std::string word;
  fileStream >> word;

  if (!(word.find_first_of(";") == word.size() - 1))
    throw std::runtime_error("no ; at the end of line");

  word.resize(word.size() - 1);

  if (word.empty()) {
    throw std::runtime_error("empty entry");
    return;
  }

  for (size_t i = 0; i < serverVector->size(); ++i) {
    if ((*serverVector)[i].server_name == word){
      throw std::runtime_error("duplicated server_name");
      break;
    }
  }

  servers.push_back(word);
}

void InputHandler::newServerCheck(std::ifstream &fileStream, \
                                  s_serverConfig &server) {
  std::string word;
  fileStream >> word;

  if (word != "{")
    throw std::runtime_error("server should start with {");

  bool bracket = true;

  for (fileStream >> word; word != "}" && bracket; fileStream >> word) {
    if (word == "listen") {
      addPort(fileStream, server.ports);
      std::sort(server.ports.begin(), server.ports.end());

      for (size_t i = 0; i < server.ports.size() - 1; i++) {
        if (server.ports[i] == server.ports[i + 1]) {
          throw std::runtime_error("duplicate listen");
        }
      }
    } else if (word == "host") {
      if (!server.host.empty()) {
        throw std::runtime_error("duplicate host");
      }
      addToString(fileStream, server.host);

    } else if (word == "server_name") {
      serverNameAdd(fileStream, server.server_names);

      /*if (!server.server_name.empty()) {
        throw std::runtime_error("duplicate server_name");
      }*/

    } else if (word == "error_page") {
      addToMap(fileStream, server.error_page);

    } else if (word == "max_body_size") {
      if (!server.srv_max_body_size != 0)
        throw std::runtime_error("duplicate loc_max_body_size");
      addToSizeT(fileStream, server.srv_max_body_size);

    } else if (word == "location") {
      s_locationConfig newLocation;
      newLocation.autoindex = false;
      newLocation.cgi_php = false;
      addLocation(fileStream, newLocation);
      server.location.push_back(newLocation);

    } else if (word == "}") {
      bracket = false;
    } else {
      throw std::runtime_error("BAD FORMAT CONFIG");
    }
  }
}

bool is_empty(std::ifstream &fileStream) {
  return (fileStream.tellg() == std::ifstream::traits_type::eof());
}

void InputHandler::checkConfFile(char *fileArg) {
  std::ifstream fileStream(fileArg);

  if (fileStream.fail())
    throw(std::runtime_error("fileStream init Error"));

  if (fileStream.peek() == std::ifstream::traits_type::eof())
      throw std::runtime_error("empty file");
  std::string word;

  while (fileStream >> word) {
    if (word == "server") {
      s_serverConfig newServer;
      newServer.srv_max_body_size = SIZE_T_MAX;
      newServerCheck(fileStream, newServer);
      if (newServer.location.empty())
        throw std::runtime_error("Server need at least one location");
      serverVector->push_back(newServer);
    } else {
      throw std::runtime_error("wrong word in config file");
    }
  }
}

void InputHandler::printMap(std::map<int, std::string> mapi) {
  std::map<int, std::string>::iterator it = mapi.begin();

  for (; it != mapi.end(); ++it) {
    std::cout << "    value: " << it->first;
    std::cout << " place: " << it->second << std::endl;
  }
}

void InputHandler::printLocations(const std::vector<s_locationConfig> &location) {
  for (size_t i = 0; i <= location.size() - 1; i++) {
    s_locationConfig toprint = location[i];
    std::cout << " location: " << toprint.location << std::endl;
    std::cout << "    autoindex: " << toprint.autoindex << std::endl;
    std::cout << "    index " << toprint.index << std::endl;
    std::cout << "    root: " << toprint.root << std::endl;
    std::cout << "    cgi_extension: " << toprint.cgi_extension << std::endl;
    std::cout << "    cgi_path: " << toprint.cgi_path << std::endl;
    std::cout << "    Cgi_php: " << toprint.cgi_php << std::endl;
    if(toprint.loc_max_body_size != SIZE_T_MAX){
      std::cout << "    loc_max_body_size: " << toprint.loc_max_body_size << std::endl;
    }

    for (size_t i = 0; i < toprint.allowed_method.size(); ++i) {
      std::cout << "    allowed_method: ";
      std::cout << toprint.allowed_method[i] << std::endl;
    }

    std::cout <<"    redirect : " << toprint.redirect.first;
    std::cout << " " << toprint.redirect.second << std::endl;
    std::cout << "\n" << std::endl;
  }
}

void InputHandler::printServers() {
  for (size_t i = 0; i <= (*serverVector).size() - 1; i++) {
    s_serverConfig toprint = (*serverVector)[i];
      std::cout << "Server number:" << i << std::endl;
    
    for (size_t i = 0; i < toprint.ports.size(); i++) {
      std::cout << "port: " << toprint.ports[i] << std::endl;
    }

    std::cout << "host: " << toprint.host << std::endl;

    for(size_t j = 0; j < toprint.server_names.size(); j++) {
      std::cout << "server_name: " << toprint.server_names[j] << std::endl;
    }

    if(toprint.srv_max_body_size != SIZE_T_MAX){
      std::cout << "    loc_max_body_size: " << toprint.srv_max_body_size << std::endl;
    }

    printMap(toprint.error_page);

    if (!toprint.location.empty())
      printLocations(toprint.location);

    std::cout << "\n" << std::endl;
  }
}

InputHandler::InputHandler(char* fileArg) {
  serverVector = new std::vector<s_serverConfig>;
  checkConfFile(fileArg);
}

InputHandler::~InputHandler(void) {
  delete serverVector;
}

