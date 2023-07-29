/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:40:49 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/28 08:59:33 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/socketClass.hpp"
#include "../include/input/InputHandler.hpp"

int main(int argc, char **argv) {
  if (!InputHandler::check_args(argc, const_cast<const char **>(argv))) {
    std::cout << "[USAGE]:  ./webserv <configuration file>.conf" << std::endl;
    return(1);
  }
//  create factorys
    TCPServerSocket serverSocket;
    std::cout << "MAX EVENTS: " << MAX_EVENTS << std::endl;
//  bind to choosen port
    serverSocket.bindAndListen();
    std::cout << "Server is listening on port " << G_PORT << std::endl;

        serverSocket.handleConnections();
        // serverSocket.handleExistingConnections();
    return 0;
}