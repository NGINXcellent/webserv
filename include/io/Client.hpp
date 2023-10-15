/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 17:56:00 by lfarias-          #+#    #+#             */
/*   Updated: 2023/10/13 02:41:59 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <vector>

#include "../http/Server.hpp"
#include "../http/HttpResponse.hpp"
#include "../http/HttpRequest.hpp"

class Client {
 public:
  Client(int conFd, Server *destServer, int port, time_t connectionStart);
  ~Client(void);

  std::string&		getBuffer(void);
  HttpRequest*		getRequest(void);
  HttpResponse*		getResponse(void);
  time_t		getTimeout(void);
  Server*		getServer(void);
  int			getPort(void);
  int			getCgiPid(void);
  void			setCgiPid(int pid);
  int			getCgiFd(void);
  void			setCgiFd(int fd);
  void			reset(void);
  std::string		buffer;
  bool			isReady;

 private:
  int			connectionFd;
  int			port;
  Server*		server;
  long long		connectionTimeout;
  HttpRequest*		request;
  HttpResponse*		response;
  int			cgiPid;
  int			cgiFd;
};
#endif
