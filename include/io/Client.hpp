/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 17:56:00 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/24 15:14:50 by lfarias-         ###   ########.fr       */
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

 private:
  int									connectionFd;
  int									port;
  std::string					buffer;
  Server*							server;
  long long						connectionTimeout;
};
#endif
