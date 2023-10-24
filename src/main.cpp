/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:40:49 by dvargas           #+#    #+#             */
/*   Updated: 2023/10/19 17:39:09 by dvargas          ###   ########.fr       */
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

  try {
    InputHandler input(argv[1]);
    input.printServers();
    Controller controller(input);
    controller.init();
  }
  catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
    return (1);
  }

  return 0;
}


/*
  CGI TODO:
  1- [ ]remover logica de cgi do controller e adicionar em process no server
  2- [x]identificar quando a requisição é de um cgi - DONE !
  3- [ ]verificar montagem da resposta no caso de cgi - preciso que o body
          de response receba o conteúdo que vem do fd do pipe executado
  4- [ ]implementar cgi post
  5- [ ]implementar cgi get
  6- [ ]testar cgi
  7- [ ]pensar sobre a logica de path, se vai ficar no config ou rodar tudo que for .php direto
  8- [ ]adicionar um poll para os fds do cgi
  9- [ ]logica do waitpid
*/