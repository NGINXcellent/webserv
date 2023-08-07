/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Controller.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:51:31 by lfarias-          #+#    #+#             */
/*   Updated: 2023/08/06 22:45:21 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/socket/Controller.hpp"

Controller::Controller(const InputHandler &input) {
  for (size_t i = 0; i < input.serverVector->size(); i++) {
    Server *server = new Server(input.serverVector[]);
  }
}

void Controller::init(void) {

  for (size_t i = 0; i < ; i++) {
    socket->bindAndListen();
    std::cout << "Server is listening on port: " << port << " ..." << std::endl;

    socket->handleConnections(this);
  }
}
