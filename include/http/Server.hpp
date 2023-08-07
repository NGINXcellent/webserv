/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:23:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/06 22:22:26 by lfarias-         ###   ########.fr       */
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
  Server(struct s_serverConfig);
  ~Server(void);

  std::string      process(char *buffer);
  void             resolve(HttpRequest *request, HttpResponse *response);
  void             get(HttpRequest *request, HttpResponse *response);
  void             head(HttpRequest *request, HttpResponse *response);
  void             post(HttpRequest *request, HttpResponse *response);
  void             del(HttpRequest *request, HttpResponse *response);

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
};
#endif
