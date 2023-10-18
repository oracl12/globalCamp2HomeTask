#pragma once

#include "socket_utils.h"
#include "../game.h"
#include <thread>
#include <mutex>

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