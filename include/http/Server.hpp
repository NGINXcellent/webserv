/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:23:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/05 06:21:05 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

#include "../io/Controller.hpp"
#include "./HttpRequest.hpp"
#include "./HttpResponse.hpp"
#include "./HttpStatus.hpp"
#include "../io/TcpServerSocket.hpp"
#include "../config/InputHandler.hpp"
#include "../io/Client.hpp"
#include "./HttpStatus.hpp"

#include <cstddef>
#include <string>
#include <map>
#include <vector>

class Controller;

class Server {
 public:
  Server(const struct s_serverConfig& config);
  ~Server(void);

  HttpStatusCode            process(Client* client, HttpRequest *req, HttpResponse *res);
  HttpStatusCode  resolve(Client* client, HttpRequest *request, HttpResponse *response);
  HttpStatusCode  get(Client* client, HttpRequest *request, HttpResponse *response);
  HttpStatusCode  post(HttpRequest *request, HttpResponse *response);
  HttpStatusCode  del(HttpRequest *request, HttpResponse *response);
  int             getPort(void);
  std::string     getHost(void);
  std::string     getServerName(void);
  HttpStatusCode  getCGI(Client* client, HttpRequest *request);
  void addDescriptorToEpoll(int fd);
  HttpStatusCode postCGI(HttpRequest *request, HttpResponse *response);
  char** createCGIEnv(HttpRequest *request);
  void setEpollfd(int epollfd);
  void setControllerPtr(Controller* controllerPtr);

 private:
  size_t                        port;
  std::string                   host;
  std::string                   server_name;
  size_t                        srv_max_body_size;
  int                           connection_fd;
  TCPServerSocket               *socket;
  std::map<int, std::string>    error_pages;
  std::vector<s_locationConfig> locations;
  Controller* controllerPtr;

  Server(const Server& f);
  Server& operator=(const Server& t);

  std::string      createLocation(char *buffer);
};
#endif
