/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 17:56:00 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/21 01:51:01 by lfarias-         ###   ########.fr       */
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

  std::vector<char>& getBuffer(void);
  HttpResponse*  getResponse(void);
  time_t getTimeout(void);
  Server *getServer(void);
  int getPort(void);
  void reset(void);

 private:
  int 			connectionFd;
  int 			port;
  std::vector<char> 	buffer;
  Server* 		server;
  long long 		connectionTimeout;
};
#endif
