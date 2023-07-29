/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:40:49 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/28 22:22:27 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* #include "../inc/socketClass.hpp"
#include "../include/http/HttpResponse.hpp" */

#include "../include/input/InputHandler.hpp"
#include "../include/http/Server.hpp"

#include <iostream>

int main(int argc, char **argv) {
  if (!InputHandler::check_args(argc, const_cast<const char **>(argv))) {
    std::cout << "[USAGE]:  ./webserv <configuration file>.conf" << std::endl;
    return(1);
  }
  Server::startupAndListen();
  return 0;
}
