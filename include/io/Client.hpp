/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 17:56:00 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/05 17:51:50 by lfarias-         ###   ########.fr       */
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
};
#endif
