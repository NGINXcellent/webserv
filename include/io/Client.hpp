/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 17:56:00 by lfarias-          #+#    #+#             */
/*   Updated: 2023/11/04 08:40:28 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <vector>

#include "../http/Server.hpp"
#include "../http/HttpResponse.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/HttpStatus.hpp"

class Client {
 public:
  Client(int conFd, Server *destServer, int port, time_t connectionStart, std::string kind);
  Client(int conFd, Server *destServer, int port, time_t connectionStart, std::string kind, HttpRequest* request, HttpResponse* response);
  ~Client(void);

  std::string&		getBuffer(void);
  HttpRequest*		getRequest(void);
  HttpResponse*		getResponse(void);
  time_t		getTimeout(void);
  Server*		getServer(void);
  void      setCgiClient(Client* client);
  Client*   getCgiClient(void);
  int			getPort(void);
  std::string     getKind(void);
  void			reset(void);
  HttpStatusCode     getRequestStatus(void);
  void     setRequestStatus(HttpStatusCode status);
  void setKind(std::string kind);
  std::string		buffer;
  bool			isReady;
  void setBuffer(std::string toset);
  int getConnectionFd(void);

 private:
  int			connectionFd;
  int			port;
  Server*		server;
  long long		connectionTimeout;
  HttpRequest*		request;
  HttpResponse*		response;
  std::string      kind;
  Client*        cgiClient;
  HttpStatusCode        requestStatus;
};
#endif
