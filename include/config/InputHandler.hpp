/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 12:03:58 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/18 21:56:33 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTHANDLER_HPP
# define INPUTHANDLER_HPP

#include <vector>
#include <map>
#include <string>

// based con https://www.nginx.com/resources/wiki/start/topics/examples/full/

struct s_locationConfig {
  std::string                 location;
  std::string                 autoindex;
  std::string                 index;
  std::string                 max_body_size;
  std::string                 root;
  std::pair<int, std::string> redirect;
  std::vector<std::string>    allowed_method;
};

struct s_serverConfig {
  int                           socketfd;
  std::string                   port;
  std::string                   host;
  std::string                   server_name;  // server_name is unique
  std::string                   max_body_size;
  std::map<int, std::string>    error_page;
  std::vector<s_locationConfig> location;
};

class InputHandler {
 private:
  void serverNameAdd(std::ifstream &fileStream, std::string &string);
  void printMap(std::map<int, std::string> mapi);
  bool isAMethod(const std::string &word);
  void addLocation(std::ifstream &fileStream, s_locationConfig &newLocation);
  void addPort(std::ifstream &fileStream, std::string &string);
  void addToString(std::ifstream &fileStream, std::string &string);
  void addToMap(std::ifstream &fileStream, std::map<int, std::string> &mapi);
  void addToVector(std::ifstream &fileStream, std::vector<std::string> &vec);
  void checkConfFile(char *fileArg);
  void newServerCheck(std::ifstream &fileStream, s_serverConfig &server);
  void printLocations(const std::vector<s_locationConfig> &location);
  void addToPair(std::ifstream &fileStream, std::pair<int, std::string> &mapi);

 public:
  std::vector<s_serverConfig> *serverVector;

  InputHandler(char* fileArg);
  ~InputHandler(void);

  static bool check_args(int argc, const char **argv);
  void        printServers();
};

#endif
