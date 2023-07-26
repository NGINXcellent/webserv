#ifndef SOCKETFACTORY_HPP
#define SOCKETFACTORY_HPP

#include <cerrno>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

// Base Socket class FULL VIRTUAL CLASS
// From there we will create all types of sockets, can be TCP, or any other.
class Socket {
public:
    virtual ~Socket() {}
    virtual int bindAndListen(int port) = 0;
    virtual int acceptConnection() = 0;
    virtual int receiveData(int connection, char* buffer, int bufferSize) = 0;
    virtual void sendData(int connection, const char* data, int dataSize) = 0;
    virtual void closeConnection(int connection) = 0;
};

// Concrete Server Socket
// inehrate functions from socket and implements its own protocol dealing with calls
class TCPServerSocket : public Socket {
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

// Server Socket Factory
class ServerSocketFactory {
public:
    virtual ~ServerSocketFactory() {}
    virtual Socket* createSocket() const = 0;
};

// Concrete Server Socket Factory
// we instantiate this to create our TCPSOCKETS or other kind of socket we want :)
// or any other kind of socketfactory
class TCPServerSocketFactory : public ServerSocketFactory {
public:
    Socket* createSocket() const;
};

#endif