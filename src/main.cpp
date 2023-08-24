/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:40:49 by dvargas           #+#    #+#             */
/*   Updated: 2023/08/24 18:04:59 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config/InputHandler.hpp"
#include "../include/http/Server.hpp"
#include "../include/io/Controller.hpp"

#include <iostream>

int main(int argc, char **argv) {
  if (!InputHandler::check_args(argc, const_cast<const char **>(argv))) {
    std::cout << "[USAGE]:  ./webserv <configuration file>.conf" << std::endl;
    return(1);
  }

  //try {
    InputHandler input(argv[1]);
    input.printServers();
    Controller controller(input);
    controller.init();
 /* catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
    return (1);
  }*/

  return 0;
}
