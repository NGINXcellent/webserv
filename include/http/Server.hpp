/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:23:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/27 15:26:57 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

#include "./HttpRequest.hpp"
#include "./HttpResponse.hpp"
#include "../io/TcpServerSocket.hpp"
#include "../config/InputHandler.hpp"

#include <cstddef>
#include <string>
#include <map>
#include <vector>

class Server {
 public:
  Server(const struct s_serverConfig& config);
  ~Server(void);

  HttpResponse     *process(std::string &buffer);
  int             resolve(HttpRequest *request, HttpResponse *response);
  int             get(HttpRequest *request, HttpResponse *response);
  int             post(HttpRequest *request, HttpResponse *response);
  //int             head(HttpRequest *request, HttpResponse *response);
  int             del(HttpRequest *request, HttpResponse *response);
  int             getPort(void);

 private:
  size_t                        port;
  std::string                   host;
  std::string                   server_name;
  size_t                        srv_max_body_size;
  int                           connection_fd;
  TCPServerSocket               *socket;
  std::map<int, std::string>    error_pages;
  std::vector<s_locationConfig> locations;

  Server(const Server& f);
  Server& operator=(const Server& t);

  std::string      createLocation(char *buffer);
};
#endif
