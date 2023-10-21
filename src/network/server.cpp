#include <iostream>
#include <vector>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <winsock2.h>
#include <windows.h>

#include "../../headers/network/server.h"
#include "../../headers/player.h"
#include "../../headers/enemy.h"

void printMatrix(int matrix[][10])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			std::cout << matrix[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

void Server::runServer()
{
    while (true)
    {
        std::cout << "WE ARE IN RUNSERVER" << std::endl;
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);

        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Error accepting connection." << std::endl;
            Sleep(3000);
            break;
        }

        std::clog << "Client connected." << std::endl;

        // check if connection exists each second by exchaning ready values
        readyCheck(clientSocket);

        // after that exchange
        sendReceivePrivateMatrix(clientSocket);

        std::clog << "Matrix was exchange successfully" << std::endl;
        // server goes first
        // after our shoot forceExhangeMatrix,
        // here forceExhangeMatrix requests sending public matrix by tcp

        // client goes next
        // from start it waites to receive matrix, after this receiment, it sets playerStep to true
        // after that after all shootings etc.. we are sending public matrix of enemy

        // server at this time is waiting to receive and after receivement it changes playerStep again to true
        
        bool sent = false;

        while (true) 
        {
            {
			    std::unique_lock<std::mutex> lock(exchangeMutex);
                if (forceExhangeMatrix) 
                {
                    playerStep = false;
                    sendPublicMatrix(clientSocket);
                    forceExhangeMatrix = false;
                    sent = true;
                }
            }

            if (sent)
            {
                receivePublicMatrix(clientSocket);
                playerStep = true;
                sent = false;
            }
        }
    }
}

void Server::readyCheck(int clientSocket)
{
    bool receivedEnemyesReady = false;
    while (true)
    {
        if (ready && !receivedEnemyesReady) { // then we are waiting for ourselves
            const char* dataSent = "ready";
            int bytesSent = send(clientSocket, dataSent, strlen(dataSent), 0);
            if (bytesSent <= 0)
            {
                Sleep(1000);
                std::cerr << "CLIENT disconected" << std::endl;
                break;
            }
            receivedEnemyesReady = true;
        }

        if (receivedEnemyesReady) { // here we just waiting for server to be ready
            char buffer[8];
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            buffer[bytesRead] = '\0';
            if (bytesRead <= 0)
            {
                Sleep(1000);
                std::cerr << "Client disconnected" << std::endl;
                break;
            }
            std::clog << "BOTH server and client are ready" << std::endl;
            break;
        }
    }
}

void Server::sendReceivePrivateMatrix(int clientSocket)
{
    int matrix[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = Player::getPrivateMatrix()[i][j];
        }
    }

    int bytesSent = send(clientSocket, (char*)matrix, sizeof(matrix), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Send failed" << std::endl;
    } else {
        std::cout << "Matrix sent successfully" << std::endl;
    }

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
}

void Server::sendPublicMatrix(int clientSocket)
{
    int matrix[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = Enemy::getPublicMatrix()[i][j];
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

void Server::receivePublicMatrix(int clientSocket)
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

Server::Server(int port)
{
    std::clog << "STARTING SERVER" << std::endl;

    WSAStartUp();

    serverSocket = initsSocket();

    bindSocket(serverSocket, port);

    listenToSocket(serverSocket);

    std::clog << "Server is listening on port " << port << "..." << std::endl;

    serverThread = std::thread(&runServer, this);
}

Server::~Server()
{
    if (serverThread.joinable()) {
        serverThread.join();
    }

    closesocket(serverSocket);
    WSACleanup();
}