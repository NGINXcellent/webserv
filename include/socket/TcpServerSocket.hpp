/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServerSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:29:55 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/30 21:03:19 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSERVERSOCKET_HPP
#define TCPSERVERSOCKET_HPP

#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#define MAX_EVENTS 10

class Server;

class TCPServerSocket {
 public:
  TCPServerSocket(unsigned int port);
  ~TCPServerSocket(void);

  int   bindAndListen(void);
  void  handleConnections(Server *server);
  // void handleExistingConnections();
  // static void handleSIGINT(int);
  int   acceptConnection(void);
  int   receiveData(int connection, char* buffer, int bufferSize);
  void  sendData(int connection, const char* data, int dataSize);
  void  closeConnection(int connection);
  int   isNewClient(void);
  void  addNewConnection(void);

 private:
  int                 sockfd;
  int                 epollfd;
  unsigned int        hostPort;
  std::vector<int>    connections;
  struct epoll_event  events[MAX_EVENTS];
};

#endif
