/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketClass.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:29:55 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/30 08:29:58 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETCLASS_HPP
#define SOCKETCLASS_HPP

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
#define G_PORT 9999
// Sockets functions
class TCPServerSocket {
 public:
  TCPServerSocket();
  ~TCPServerSocket();
  int bindAndListen();
  void handleConnections();
  // void handleExistingConnections();
  // static void handleSIGINT(int);
  int acceptConnection();
  int receiveData(int connection, char* buffer, int bufferSize);
  void sendData(int connection, const char* data, int dataSize);
  void closeConnection(int connection);
  int isNewClient();
  void addNewConnection();

 private:
  int sockfd;
  int epollfd;
  std::vector<int> connections;
  struct epoll_event events[MAX_EVENTS];
  // int currentFd;
};

#endif