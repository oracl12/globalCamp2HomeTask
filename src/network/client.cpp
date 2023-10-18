#include <iostream>
#include <cstring>
#include <thread>
#include "../../headers/network/client.h"
#include "../../headers/player.h"
#include "../../headers/enemy.h"

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <arpa/inet.h>
#endif

extern void printMatrix(int matrix[][10]);

void Client::runClient()
{
    readyCheck();
    // exchange
    receiveSendPrivateMatrix();
    std::clog << "Matrix was exchange successfully" << std::endl;

    bool received = false;
    // // after that exchange each time when step, so
    while (true) {
        {
            // on receive of server step -> playerStep = true;
            // send-> ... -> receive
            if (!received)
            {
                receivePublicMatrix();
                playerStep = true;
                received = true;
            }

			{
                std::unique_lock<std::mutex> lock(exchangeMutex);
                if (forceExhangeMatrix) 
                {
                    sendPublicMatrix();
                    forceExhangeMatrix = false;
                    playerStep = false;
                    received = false;
                }
            }
        }
    }
}

void Client::readyCheck()
{
    bool sentReadyToEnemy = false;
    while (true)
    {
        if (!sentReadyToEnemy) {
            char buffer[8];
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            buffer[bytesRead] = '\0';
            if (bytesRead <= 0)
            {
                Sleep(1000);
                std::cerr << "Server is dead" << std::endl;
                break;
            }
            sentReadyToEnemy = true;
        }

        if (ready && sentReadyToEnemy) { // then we are sending to server
            const char* dataSent = "ready";
            int bytesSent = send(clientSocket, dataSent, strlen(dataSent), 0);
            if (bytesSent <= 0)
            {
                Sleep(1000);
                std::cerr << "Server is dead" << std::endl;
                break;
            }
            std::clog << "BOTH server and client are ready" << std::endl;
            break;
        }
    }
}

void Client::receivePublicMatrix()
{
    int matrix[10][10];
    std::cout << "Public Matrix waiting to receive" << std::endl;
    int bytesReceived = recv(clientSocket, (char*)matrix, sizeof(matrix), 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Receive failed" << std::endl;
    } else {
        std::cout << "Public Matrix received successfully" << std::endl;

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                std::cout << matrix[i][j] << ' ';
                Player::getPublicMatrix()[i][j] = matrix[i][j];
            }
            std::cout << std::endl;
        }

        std::cout << "ENEMYs public:" << std::endl;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                std::cout << Player::getPublicMatrix()[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }
}

void Client::sendPublicMatrix()
{
    int matrix[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = Enemy::getPublicMatrix()[i][j];;
        }
    }

    std::cout << "Public Matrix is about to sent" << std::endl;

    int bytesSent = send(clientSocket, (char*)matrix, sizeof(matrix), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Send failed" << std::endl;
    } else {
        std::cout << "Public Matrix sent successfully" << std::endl;
    }
}

void Client::receiveSendPrivateMatrix()
{
    int matrix[10][10];
    int bytesReceived = recv(clientSocket, (char*)matrix, sizeof(matrix), 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Receive failed" << std::endl;
    } else {
        std::cout << "Matrix received successfully" << std::endl;

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                std::cout << matrix[i][j] << ' ';
                Enemy::getPrivateMatrix()[i][j] = matrix[i][j];
            }
            std::cout << std::endl;
        }

        std::cout << "ENEMYs:" << std::endl;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                std::cout << Enemy::getPrivateMatrix()[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = Player::getPrivateMatrix()[i][j];;
        }
    }

    int bytesSent = send(clientSocket, (char*)matrix, sizeof(matrix), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Send failed" << std::endl;
    } else {
        std::cout << "Matrix sent successfully" << std::endl;
    }
}

Client::Client(int port)
{
    std::cout << "STARTING CLIENT" << std::endl;

    WSAStartupIfNeeded();

    clientSocket = initsSocket();

    connectToSocket(clientSocket, port);

    std::cout << "Connected to the server." << std::endl;

    clientThread = std::thread(&runClient, this);
}

Client::~Client()
{
    if (clientThread.joinable()) {
        clientThread.join();
    }

    closeSocket(clientSocket);
    WSACleanupIfNeeded();
}
