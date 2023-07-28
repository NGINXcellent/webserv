/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketClass.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:29:55 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/28 09:53:58 by dvargas          ###   ########.fr       */
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

#define MAX_EVENTS 10
#define G_PORT 9999
// Sockets functions
class TCPServerSocket {
public:
    TCPServerSocket();
    ~TCPServerSocket();
    int bindAndListen();
    void handleConnections();
    // void handleExistingConnections();
    // static void handleSIGINT(int);
    int acceptConnection();
    int receiveData(int connection, char* buffer, int bufferSize);
    void sendData(int connection, const char* data, int dataSize);
    void closeConnection(int connection);
    int isNewClient();
    void addNewConnection();

private:
    int sockfd;
    int epollfd;
    std::vector<int> connections;
    struct epoll_event events[MAX_EVENTS];
    // int currentFd;
};

#endif