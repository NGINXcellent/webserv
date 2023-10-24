/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:23:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/10/24 09:18:19 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

#include "./HttpRequest.hpp"
#include "./HttpResponse.hpp"
#include "./HttpStatus.hpp"
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

  void            process(std::string &buffer, HttpRequest *req, HttpResponse *res);
  void            processCgi(HttpRequest *req, int pipe[2], std::vector<std::string> &env_vars);
  HttpStatusCode  resolve(HttpRequest *request, HttpResponse *response);
  HttpStatusCode  get(HttpRequest *request, HttpResponse *response);
  HttpStatusCode  getCgi(HttpRequest *request, std::vector<std::string> &env_vars);
  HttpStatusCode  postCgi(HttpRequest *request, int pipe[2], std::vector<std::string> &env_vars);
  HttpStatusCode  post(HttpRequest *request, HttpResponse *response);
  HttpStatusCode  del(HttpRequest *request, HttpResponse *response);
  void            setupCgiVars(HttpRequest *request, std::vector<std::string> &env_vars);
  int             getPort(void);
  std::string     getHost(void);
  std::string     getServerName(void);
  int			getCgiPid(void);
  void			setCgiPid(int pid);
  int			getCgiFd(void);
  void			setCgiFd(int fd);
  bool            handleCgiGet(HttpRequest *request, char* resourceData, long long resourceSize);

 private:
  size_t                        port;
  std::string                   host;
  std::string                   server_name;
  size_t                        srv_max_body_size;
  int                           connection_fd;
  TCPServerSocket               *socket;
  std::map<int, std::string>    error_pages;
  std::vector<s_locationConfig> locations;
  int			cgiPid;
  int			cgiFd;

  Server(const Server& f);
  Server& operator=(const Server& t);

  std::string      createLocation(char *buffer);
};
#endif
