/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Controller.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:48:07 by lfarias-          #+#    #+#             */
/*   Updated: 2023/09/05 13:53:05 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTROLLER_HPP
# define CONTROLLER_HPP

#include "../config/InputHandler.hpp"
#include "../http/Server.hpp"
#include "./TcpServerSocket.hpp"
#include "./Client.hpp"

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
  int                               epollfd;
  char                              buffer[4096];
  std::map<int, Client*>            connectedClients;
  std::map<int, Server*>            serverPool;
  std::map<int, TCPServerSocket*>   socketPool;
  std::vector<epoll_event> events;

  Controller(const Controller& f);
  Controller& operator=(const Controller& t);

  // event handlers
  bool  isNewConnection(int connectionFd);
  void  addNewConnection(int connectionFd);
  void  readFromClient(int connectionFd);
  void  sendToClient(int connectionFd);
  bool  closeConnection(int connectionFd);

  // signal handler
  static void endServer();
  static void signalHandler(int signal);

  // POST handling
  bool  isHTTPRequestComplete(const std::string &request);
  bool	isChunkedBodyComplete(const std::string &body);
  bool	isMultipartBodyComplete(const std::string &body);
  bool	isUrlEncodedBodyComplete(const std::string &body, size_t cLength);
  size_t  findContentLength(const std::string& request);

  // utils
  void  initEpollEvent(struct epoll_event *ev, uint32_t flag, int fd);
  int   getPortFromFd(int connectionFd);
  int	getSocketPort(int socketFd);
  void	checkTimeOut();
};
#endif
