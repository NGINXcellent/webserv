/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:23:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/10 03:30:10 by lfarias-         ###   ########.fr       */
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

  HttpStatusCode  process(Client* client, HttpRequest *req, HttpResponse *res);
  HttpStatusCode  resolve(Client* client, HttpRequest *request, HttpResponse *response);
  HttpStatusCode  get(Client* client, HttpRequest *request, HttpResponse *response);
  HttpStatusCode  post(Client* client, HttpRequest *request, HttpResponse *response);
  HttpStatusCode  del(HttpRequest *request, HttpResponse *response);
  int             getPort(void);
  std::vector<size_t> getPorts(void);
  std::string     getHost(void);
  std::vector<std::string>     &getServerNames(void);
  HttpStatusCode  getCGI(Client* client, HttpRequest *request);
  void addDescriptorToEpoll(int fd);
  HttpStatusCode postCGI(Client* client, HttpRequest *request);
  char** createCGIEnv(HttpRequest *request);
  void setEpollfd(int epollfd);
  void setControllerPtr(Controller* controllerPtr);

 private:
  size_t                        port;
  std::vector<size_t>           ports;
  std::string                   host;
  std::string                   server_name;
  std::vector<std::string>      server_names;
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
