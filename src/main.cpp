/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:40:49 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/26 08:41:51 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/socketFactory.hpp"

int main() {
//  create factorys
    ServerSocketFactory* socketFactory = new TCPServerSocketFactory();
    Socket* serverSocket = socketFactory->createSocket();

//  bind to choosen port
    serverSocket->bindAndListen(9999);
    std::cout << "Server is listening on port 9999..." << std::endl;

//  accept conections
    int connection = serverSocket->acceptConnection();
    std::cout << "Connection established. Waiting for data..." << std::endl;

//  logic to file and data receive
    char buffer[100];
    bool connected = true;
      while (connected) {
        int bytesRead = serverSocket->receiveData(connection, buffer, 100);
        if (bytesRead <= 0) {
            // If bytesRead for <= 0, conection closed
            std::cout << "Client disconnected." << std::endl;
            connected = false;
        } else {
            std::cout << "Received data: " << buffer;
            memset(buffer, 0, sizeof(buffer));
            std::string response = "Good talking to you\n";
            serverSocket->sendData(connection, response.c_str(), response.size());
        }
    }

//  close conection and end program
    serverSocket->closeConnection(connection);
    delete serverSocket;
    delete socketFactory;

    return 0;
}