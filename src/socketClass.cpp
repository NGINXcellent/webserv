/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketClass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:40:52 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/26 16:01:34 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/socketClass.hpp"

// Constructor Server Socket
TCPServerSocket::TCPServerSocket() : sockfd(-1), connection(-1) {}

int TCPServerSocket::bindAndListen(int port) {
// 			socket function defines the type of address this socket can conect
//			socket(DOMAN, TYPE, PROTOCOL)
//			DOMAIN: AF_INET-> ipv4		TYPE: SOCK_STREAM -> conection oriented sockets like TCP
//					AF_INET6->ipv6			  SOCK_DGRAM -> not oriented sockets, UDP
//					AF_UNIX or AF_LOCAL -> Unix domain sockets
//			TYPE: specify protocol used on this socket, normally 0, the function
//					will choose protocol based on other two arguments
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

//		this structure is defined in lib <netinet/in.h>
// struct sockaddr_in {
//     short sin_family;           // ADDRESS FAMILY (AF_INET para IPv4)
//     unsigned short sin_port;    // PORT NUMBER
//     struct in_addr sin_addr;    // IPADDRESS (INADDR_ANY, INADDR_LOOPBACK,
//									inet_addr("xxx.xxx.xxx.xxx") set ip you want)
//     char sin_zero[8];           // NOT USED
// };
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET; // Same as before, Ipv4
    sockaddr.sin_addr.s_addr = INADDR_ANY; // set every address interface 0.0.0.0
    sockaddr.sin_port = htons(port); //make convertion to big-endian or little endial (htonl)

//	after setup, the function bind uses struct to bind to specific port and IP if fail
//  it returns an -1 and error in cout and exit_failure flag(1)
    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind to port " << port << ". errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
//	if we can bind to ip and port, now we use the function listen(socket, how many connections we handle), if listen returns -1 we send a error and exit_failure flag (1)
    if (listen(sockfd, 10) < 0) {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

// responsable for accept the conection, can be in the other function but like this i think
// is more organized. "copy" the sockfd listener to a new FD of reader.
int TCPServerSocket::acceptConnection() {
    sockaddr_in sockaddr;
    socklen_t addrlen = sizeof(sockaddr);
    connection = accept(sockfd, (struct sockaddr*)&sockaddr, &addrlen);
    if (connection < 0) {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    return connection;
}

// return data received from conection fd
int TCPServerSocket::receiveData(int connection, char* buffer, int bufferSize) {
    int bytesRead = read(connection, buffer, bufferSize);
    return bytesRead;
}

// send data to conection fd
void TCPServerSocket::sendData(int connection, const char* data, int dataSize) {
    send(connection, data, dataSize, 0);
}

// close conection fd
void TCPServerSocket::closeConnection(int connection) {
    close(connection);
}

// simple destructor
TCPServerSocket::~TCPServerSocket() {
    if (connection != -1) {
        close(connection);
    }
    if (sockfd != -1) {
        close(sockfd);
    }
}