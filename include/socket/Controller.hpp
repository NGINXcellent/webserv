/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Controller.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:48:07 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/18 14:49:49 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONTROLLER_HPP
# define CONTROLLER_HPP

#include "../input/InputHandler.hpp"
#include "../http/Server.hpp"
#include "./TcpServerSocket.hpp"

#include <csignal>
#include <vector>
#include <map>

#define TIMEOUT 60

class Controller {
 public:
  Controller(const InputHandler &input);
  ~Controller(void);

  void  init(void);
  void  handleConnections(void);

 private:
  std::vector<int>                connections;
  std::map<int, Server*>          serverPool;
  std::map<int, TCPServerSocket*> socketPool;
  std::map<int, char *>           bufferPool;  // connectionFD, buffer
  std::map<int, time_t>           timeoutPool;
  int                             epollfd;
  struct epoll_event              events[MAX_EVENTS];

  Controller(const Controller& f);
  Controller& operator=(const Controller& t);

  // event handlers
  bool  isNewConnection(int currentFD);
  void  addNewConnection(int socketFD);
  void  readFromClient(int currentFd);
  void  sendToClient(int currentFd);
  void  closeConnection(int currentFd);

  //signal handler
  static void endServer();
  static void signalHandler(int signal);

  int findConnectionSocket(int socketFD);
  void checkTimeOut();
};
#endif
