/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Controller.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:48:07 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/08 18:16:48 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONTROLLER_HPP
# define CONTROLLER_HPP

#include "../input/InputHandler.hpp"
#include "../http/Server.hpp"
#include "./TcpServerSocket.hpp"

#include <vector>
#include <map>

class Controller {
 public:
  Controller(const InputHandler &input);
  ~Controller(void);

  void  init(void);
  void  handleConnections(void);

 private:
  std::vector<Server*>          serverList;
  std::vector<int>              connections;
  std::vector<TCPServerSocket*> socketList;
  std::map<int, char *>         bufferList;  // connectionFD, buffer
  int                           epollfd;
  struct epoll_event            events[MAX_EVENTS];

  Controller(const Controller& f);
  Controller& operator=(const Controller& t);

  // event handlers
  bool  isNewConnection(int currentFD);
  void  addNewConnection(int socketFD);
  void  writeToBuffer(int currentFd);
  void  sendToClient(int currentFd);
  void  closeConnection(int currentFd);
};
#endif
