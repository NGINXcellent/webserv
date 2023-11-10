/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServerSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:29:55 by dvargas           #+#    #+#             */
/*   Updated: 2023/11/10 04:27:48 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSERVERSOCKET_HPP
#define TCPSERVERSOCKET_HPP

#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <string>
#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>

#define MAX_EVENTS 1000

class Server;

class TCPServerSocket {
 public:
  TCPServerSocket(unsigned int port, const std::string &host);
  ~TCPServerSocket(void);

  int		bindAndListen(void);
  int		acceptConnection(void);
  unsigned long ft_inet_addr(const char *ipAddrString);
  int		receiveData(int connection, char* buffer, int bufferSize);
  int		sendData(int connection, const char* data, int dataSize);
  void		closeConnection(int connection);
  int		getFD(void);
  unsigned int	getPort(void);

 private:
  int                 sockfd;
  unsigned int        hostPort;
  std::string	      hostAddr;
};

#endif
