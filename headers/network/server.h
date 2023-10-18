#pragma once

#include "socket_utils.h"
#include "../game.h"
#include <mutex>
#include <thread>

extern bool ready;
extern bool forceExhangeMatrix;
extern bool playerStep;
extern std::mutex exchangeMutex;

class Server : public SocketUtil
{
public:
    Server(int port);

    ~Server();

	Server(const Server &) = delete;

	Server &operator=(const Server &) = delete;

private:
    void runServer();
    void readyCheck(int clientSocket);
    void sendReceivePrivateMatrix(int clientSocket);
    void sendPublicMatrix(int clientSocket);
    void receivePublicMatrix(int clientSocket);
    int serverSocket;
    std::thread serverThread;
};