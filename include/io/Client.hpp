/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 17:56:00 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/27 10:31:03 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <vector>

#include "../http/HttpResponse.hpp"
#include "../http/Server.hpp"

class Client {
 public:
  Client(int conFd, Server *destServer, int port, time_t connectionStart);
  ~Client(void);

  std::string&		getBuffer(void);
  HttpResponse*		getResponse(void);
  time_t					getTimeout(void);
  Server*					getServer(void);
  int							getPort(void);
  void						reset(void);
  std::string					buffer;
  bool            isReady;

 private:
  int									connectionFd;
  int									port;
  // std::string					buffer;
  Server*							server;
  long long						connectionTimeout;
};
#endif
