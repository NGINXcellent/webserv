/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:40:49 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/28 16:53:32 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/socketClass.hpp"
#include "../include/input/InputHandler.hpp"
#include "../include/http/HttpResponse.hpp"

int main(int argc, char **argv) {
  if (!InputHandler::check_args(argc, const_cast<const char **>(argv))) {
    std::cout << "[USAGE]:  ./webserv <configuration file>.conf" << std::endl;
    return(1);
  }
//  create factorys
    TCPServerSocket serverSocket;

//  bind to choosen port
    int port = 8080;
    serverSocket.bindAndListen(port);
    std::cout << "Server is listening on port: " << port << " ..." << std::endl;

//  accept conections
    int connection = serverSocket.acceptConnection();
    std::cout << "Connection established. Waiting for data..." << std::endl;

//  logic to file and data receive
    char buffer[100];
    bool connected = true;
      while (connected) {
        int bytesRead = serverSocket.receiveData(connection, buffer, 100);
        /*if (bytesRead <= 0) {
            // If bytesRead for <= 0, conection closed
            std::cout << "Client disconnected." << std::endl;
            connected = false;
        } else {*/
        if (bytesRead != 0) {
          std::cout << "Received data: " << buffer;
          memset(buffer, 0, sizeof(buffer));
          //std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello, World!";
          //serverSocket.sendData(connection, response.c_str(), response.size());
          HttpResponse response;
          std::string headers = response.getHeaders();
          std::cout << headers << std::endl;
          serverSocket.sendData(connection, headers.c_str(), headers.size());
          serverSocket.sendData(connection, "Hello World", 12);
        }
        //std::cout << "eu nao to maluco!" << std::endl;
    }
//  close conection and end program
    serverSocket.closeConnection(connection);
    return 0;
}
