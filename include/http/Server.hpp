/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:23:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/18 16:37:07 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

#include "./HttpRequest.hpp"
#include "./HttpResponse.hpp"
#include "../socket/TcpServerSocket.hpp"
#include "../input/InputHandler.hpp"

#include <cstddef>
#include <string>
#include <map>
#include <vector>

class Server {
 public:
  Server(const struct s_serverConfig& config);
  ~Server(void);

  std::string      process(const std::vector<char> &buffer);
  void             resolve(HttpRequest *request, HttpResponse *response);
  void             get(HttpRequest *request, HttpResponse *response);
  void             head(HttpRequest *request, HttpResponse *response);
  void             post(HttpRequest *request, HttpResponse *response);
  void             del(HttpRequest *request, HttpResponse *response);
  int              getPort(void);

 private:
  size_t                        port;
  std::string                   host;
  std::string                   server_name;
  size_t                        max_body_size;
  int                           connection_fd;
  TCPServerSocket               *socket;
  std::map<int, std::string>    error_pages;
  std::vector<s_locationConfig> locations;

  Server(const Server& f);
  Server& operator=(const Server& t);

  std::string      createLocation(char *buffer);
};
#endif
