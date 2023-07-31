/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:23:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/30 21:12:29 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

#include "./HttpRequest.hpp"
#include "./HttpResponse.hpp"
#include "../socket/TcpServerSocket.hpp"

#include <string>

class Server {
 public:
  Server(unsigned int port);
  ~Server(void);

  void        start();
  std::string process(char *buffer);
  void        resolve(HttpRequest *request, HttpResponse *response);
  void        get(HttpRequest *request, HttpResponse *response);
  void        post(HttpRequest *request, HttpResponse *response);
  void        del(HttpRequest *request, HttpResponse *response);

 private:
  unsigned int      port;
  int               connection_fd;
  TCPServerSocket   *socket;

  Server(const Server& f);
  Server& operator=(const Server& t);
};
#endif
