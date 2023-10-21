#include <iostream>
#include <cstring>
#include <thread>
#include "../../headers/network/client.h"
#include "../../headers/player.h"
#include "../../headers/enemy.h"

#include <winsock2.h>
#include <windows.h>

extern void printMatrix(int matrix[][10]);

void Client::runClient()
{
    readyCheck();

    receiveSendPrivateMatrix();

    bool received = false;

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
                std::cerr << "CLIENT: SERVER dead" << std::endl;
                break;
            }
            sentReadyToEnemy = true;
        }

        if (ready && sentReadyToEnemy) { // then we are sending to server
            const char* dataSent = "ready";
            int bytesSent = send(clientSocket, dataSent, strlen(dataSent), 0);
            if (bytesSent <= 0)
            {
                Sleep(1);
                std::cerr << "SERVER: dead" << std::endl;
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
    std::cout << "CLIENT: Public Matrix: waiting to receive" << std::endl;
    int bytesReceived = recv(clientSocket, (char*)matrix, sizeof(matrix), 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Receive failed" << std::endl;
    } else {
        std::cout << "CLIENT: Public Matrix: received successfully" << std::endl;

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                std::cout << matrix[i][j] << ' ';
                Player::getPublicMatrix()[i][j] = matrix[i][j];
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

    std::cout << "CLIENT: Public Matrix: about to sent" << std::endl;

    int bytesSent = send(clientSocket, (char*)matrix, sizeof(matrix), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "CLIENT: Public Matrix: send failed" << std::endl;
    } else {
        std::cout << "CLIENT: Public Matrix: sent successfully" << std::endl;
    }
}

void Client::receiveSendPrivateMatrix()
{
    int matrix[10][10];
    int bytesReceived = recv(clientSocket, (char*)matrix, sizeof(matrix), 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "CLIENT: Private Matrix: receive failed" << std::endl;
    } else {
        std::cout << "CLIENT: Private Matrix: received successfully" << std::endl;

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                std::cout << matrix[i][j] << ' ';
                Enemy::getPrivateMatrix()[i][j] = matrix[i][j];
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
        std::cerr << "CLIENT: Private Matrix: send failed" << std::endl;
    } else {
        std::cout << "CLIENT: Private Matrix: sent successfully" << std::endl;
    }
}

Client::Client(int port)
{
    std::cout << "STARTING CLIENT" << std::endl;

    WSAStartUp();

    clientSocket = initsSocket();

    connectToSocket(clientSocket, port);

    std::cout << "CLIENT: Connected to the server." << std::endl;

    clientThread = std::thread(&runClient, this);
}

Client::~Client()
{
    std::cout << "CLIENT: SHUTTING DOWN" << std::endl;
    if (clientThread.joinable()) {
        clientThread.join();
    }

    closesocket(clientSocket);
    WSACleanup();
}
