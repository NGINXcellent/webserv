/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketClass.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:29:55 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/27 09:16:46 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETCLASS_HPP
#define SOCKETCLASS_HPP

#include <cerrno>
#include <vector>
#include <csignal>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/epoll.h>

// int g_currentFd;
    const int MAX_EVENTS = 10;
    const int G_PORT = 9999;
// Sockets functions
class TCPServerSocket {
public:
    TCPServerSocket();
    ~TCPServerSocket();
    int bindAndListen(int port);
    void handleConnections();
    // void handleExistingConnections();
    // static void handleSIGINT(int);
    int acceptConnection();
    int receiveData(int connection, char* buffer, int bufferSize);
    void sendData(int connection, const char* data, int dataSize);
    void closeConnection(int connection);

private:
    int sockfd;
    int epollfd;
    std::vector<int> connections;
    // int currentFd;
};

#endif