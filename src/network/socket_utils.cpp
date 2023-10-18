#include "../../headers/network/socket_utils.h"

void SocketUtil::WSACleanupIfNeeded()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

void SocketUtil::closeSocket(int socket)
{
#ifdef _WIN32
    closesocket(socket);
#else
    close(socket);
#endif
}

void SocketUtil::WSAStartupIfNeeded()
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        Sleep(1);
        exit(1);
    }
#endif
}

int SocketUtil::initsSocket()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Error creating socket." << std::endl;
        Sleep(1);
        WSACleanupIfNeeded();
        exit(1);
    }
    return clientSocket;
};

sockaddr_in SocketUtil::getServerAddr(int port)
{
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(IP_SERVER);
    return serverAddr;
}

void SocketUtil::bindSocket(int socket, int port)
{
    sockaddr_in serverAddr = getServerAddr(port);

    if (bind(socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Error binding socket." << std::endl;
        Sleep(1);
        closeSocket(socket);
        WSACleanupIfNeeded();
        exit(1);
    }
};

void SocketUtil::listenToSocket(int socket)
{
    if (listen(socket, 1) == SOCKET_ERROR)
    {
        std::cerr << "Error listening for connections." << std::endl;
        Sleep(1);
        closeSocket(socket);
        WSACleanupIfNeeded();
        exit(1);
    }
};

void SocketUtil::connectToSocket(int socket, int port)
{
    sockaddr_in serverAddr = getServerAddr(port);

    if (connect(socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Error connecting to the server." << std::endl;
        Sleep(1);
        closeSocket(socket);
        WSACleanupIfNeeded();
        exit(1);
    }
};