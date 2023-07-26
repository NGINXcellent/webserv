/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketClass.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:29:55 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/26 15:29:57 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETCLASS_HPP
#define SOCKETCLASS_HPP

#include <cerrno>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

// Sockets functions
class TCPServerSocket {
public:
    TCPServerSocket();
    ~TCPServerSocket();
    int bindAndListen(int port);
    int acceptConnection();
    int receiveData(int connection, char* buffer, int bufferSize);
    void sendData(int connection, const char* data, int dataSize);
    void closeConnection(int connection);

private:
    int sockfd;
    int connection;
};

#endif