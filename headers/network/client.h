#pragma once

#include <thread>
#include <mutex>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <cstring>
#include <thread>

#include "../../headers/player.h"
#include "../../headers/enemy.h"
#include "../game.h"
#include "../conf.h"
#include "socket_utils.h"

extern bool ready;
extern bool playerStep;
extern bool forceExhangeMatrix;
extern std::mutex exchangeMutex;

class Client : public SocketUtil
{
public:
    Client(int port);

    ~Client();

	Client(const Client &) = delete;

	Client &operator=(const Client &) = delete;
private:
    void runClient();
    void readyCheck();
    void receiveSendPrivateMatrix();
    void receivePublicMatrix();
    void sendPublicMatrix();
    int clientSocket;
    std::thread clientThread;
};