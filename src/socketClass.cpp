/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketClass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:40:52 by dvargas           #+#    #+#             */
/*   Updated: 2023/07/28 10:11:38 by dvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/socketClass.hpp"

// Constructor Server Socket
TCPServerSocket::TCPServerSocket() : sockfd(-1), epollfd(-1) {}

int TCPServerSocket::bindAndListen()
{
    // 			Create epollfd
    epollfd = epoll_create1(0);
    if (epollfd == -1)
    {
        std::cerr << "Failed to create epoll. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configurar o endereço para o socket
    sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(G_PORT);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind to port " << G_PORT << ". errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Colocar o socket em modo de escuta
    if (listen(sockfd, 10) < 0)
    {
        std::cerr << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Bind sockfd on epoll event
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event) == -1)
    {
        std::cerr << "Failed to add sockfd to epoll. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    return sockfd;
}
void TCPServerSocket:: addNewConnection(){
    int newConnection;
    struct sockaddr_in clientToAdd;
    socklen_t len = sizeof(clientToAdd);

    newConnection = accept(sockfd, (struct sockaddr*)&clientToAdd, &len);
    if (newConnection < 0) {
        std::cerr << "Failed to grab new connection. errno: " << errno << std::endl;
    }
    else {
        // try to add to epoll
        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = newConnection;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newConnection, &event) == -1) {
            std::cerr << "Failed to add new connection to epoll. errno: " << errno << std::endl;
            close(newConnection);
        }
        else {
            // add this conection to conections vector
            connections.push_back(newConnection);
        }
    }
}

void TCPServerSocket::handleConnections() {
    // create just one pool of events, its like a line, if is a new conection, add to poll,
    // if not, handle the client conection.
    while (true) {
        int numEvents = epoll_wait(epollfd, events, MAX_EVENTS, 0);
        if (numEvents == -1) {
            std::cerr << "epoll_wait error. errno: " << errno << std::endl;
            continue;
        }

        for (int i = 0; i < numEvents; ++i) {
            int currentFd = events[i].data.fd;
            // in the future, we will iterate thru servers(sockfd) so the nextline will be a funcion.
            if (currentFd == sockfd) {
                // found new conection, try to add connection
                addNewConnection();
                std::cout << "new conection from fd:" << currentFd << std::endl;
                }
            else {
                // If connection already exist
                char buffer[1024];
                int bytesRead = read(currentFd, buffer, sizeof(buffer));
                if (bytesRead <= 0) {
                    // Connection error or close, we remove from epoll
                    // signal(SIGINT, handleSIGINT); localtion is here?
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, currentFd, NULL);
                    close(currentFd);
                    for (std::vector<int>::iterator it = connections.begin(); it != connections.end(); ++it)
                    {
                        if (*it == currentFd) {
                            connections.erase(it);
                            break;
                        }
                    }
                }  else {
                    // input e output handlers
                    std::cout << "Received data from fd " << currentFd << ": " << buffer;
                    memset(buffer, 0, sizeof(buffer));
                    std::string response = "Good talking to you fd\n";
                    sendData(currentFd, response.c_str(), response.size());
                }
            }
        }
    }
}

int TCPServerSocket::receiveData(int connection, char *buffer, int bufferSize)
{
    int bytesRead = read(connection, buffer, bufferSize);
    return bytesRead;
}

// send data to conection fd
void TCPServerSocket::sendData(int connection, const char *data, int dataSize)
{
    send(connection, data, dataSize, 0);
}

// close conection fd
void TCPServerSocket::closeConnection(int connection)
{
    close(connection);
}

// simple destructor
TCPServerSocket::~TCPServerSocket()
{
    if (epollfd != -1) {
        close(epollfd);
    }
    if (sockfd != -1) {
        close(sockfd);
    }
}