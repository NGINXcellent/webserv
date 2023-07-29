/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:23:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/07/29 17:58:34 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

#include "./HttpRequest.hpp"
#include "./HttpResponse.hpp"
#include "../../inc/socketClass.hpp"

class Server {
 public:
  Server(void);
  ~Server(void);

  void  startupAndListen(void);
  void  handleConnections(void);
  void  resolve(HttpRequest *request, HttpResponse *response);
  void  get(HttpRequest *request, HttpResponse *response);
  void  post(HttpRequest *request, HttpResponse *response);
  void  del(HttpRequest *request, HttpResponse *response);

 private:
  int               port;
  TCPServerSocket   socket;
  int               connection_fd;

  Server(const Server& f);
  Server& operator=(const Server& t);
};
#endif
