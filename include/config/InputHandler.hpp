/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 12:03:58 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/10 14:52:01 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTHANDLER_HPP
# define INPUTHANDLER_HPP


#include <vector>
#include <map>
#include <string>
#include <limits>

#define SIZE_T_MAX std::numeric_limits<size_t>::max()

// based con https://www.nginx.com/resources/wiki/start/topics/examples/full/

struct s_locationConfig {
  std::string                 location;
  std::string                 autoindex;
  std::string                 index;
  size_t                      loc_max_body_size;
  std::string                 root;
  std::string                 cgi_path;
  std::string                 cgi_type;
  std::pair<int, std::string> redirect;
  std::vector<std::string>    allowed_method;
};

struct s_serverConfig {
  int                           socketfd;
  std::string                   port;
  std::string                   host;
  std::string                   server_name;  // server_name is unique
  size_t                        srv_max_body_size;
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
  void addToSizeT(std::ifstream &fileStream, size_t &size);

 public:
  std::vector<s_serverConfig> *serverVector;

  InputHandler(char* fileArg);
  ~InputHandler(void);

  static bool check_args(int argc, const char **argv);
  void        printServers();
};

#endif

