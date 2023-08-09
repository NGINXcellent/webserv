/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServerSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:29:55 by dvargas           #+#    #+#             */
/*   Updated: 2023/08/08 13:53:14 by lfarias-         ###   ########.fr       */
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
  // static void handleSIGINT(int); IMPLEMENT ON CONTROLLER!
  int   acceptConnection(void);
  int   receiveData(int connection, char* buffer, int bufferSize);
  void  sendData(int connection, const char* data, int dataSize);
  void  closeConnection(int connection);
  int   getFD(void);
  unsigned int   getPort(void);

 private:
  int                 sockfd;
  unsigned int        hostPort;
};

#endif
