#pragma once 

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 46
#endif

#define SOCKET_ERROR (-1)
#define IP_SERVER "127.0.0.1"

class SocketUtil
{
public:
    void WSACleanupIfNeeded();

    void closeSocket(int socket);

    void WSAStartupIfNeeded();

    int initsSocket();

    sockaddr_in getServerAddr(int port);

    void bindSocket(int socket, int port);

    void listenToSocket(int socket);

    void connectToSocket(int socket, int port);

    char *getIp(sockaddr_in *sin_addr);
};
