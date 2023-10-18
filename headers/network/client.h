#pragma once

#include <thread>
#include <mutex>

#include "socket_utils.h"
#include "../game.h"

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