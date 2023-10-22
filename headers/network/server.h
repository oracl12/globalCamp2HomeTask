#pragma once

#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <algorithm>
#include <cstring>
#include <winsock2.h>
#include <windows.h>

#include "socket_utils.h"
#include "../game.h"
#include "../conf.h"
#include "../../headers/player.h"
#include "../../headers/enemy.h"

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