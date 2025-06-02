/**
 * @file main.cpp
 * @author Shayan Eram
 * @brief Server managing messages and controlling the buzzer based on the received message type.
 * 
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>

#include "InputOutput.hpp"

/**
 * @brief Main function of the serverExec.
 * 
 * @return 0 if execution is successful.
 */
int main()
{
    InputOutput& io = InputOutput::getInstance();
    
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(4100);

    // Bind the socket to the specified port
    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    // Listen for incoming connections
    listen(serverSocket, 5);

    std::cout << "Waiting for connections on port 4100..." << std::endl;

    // Accept connections
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);

    // Handle communication with the connected client
    // Receive message from the client
    uint32_t messageType = 0x00000000;
    recv(clientSocket, &messageType, sizeof(messageType), 0);

    std::string filenameK = "known.txt";
    std::string filenameU = "Uknown.txt";

    std::vector<dataBuz> dataTxt;
    // Handle the received message
    switch (messageType)
    {
    case io.LEARN:
        std::cout << "Received LEARN message : Nothing to do!" << std::endl;
        break;

    case io.KNOWN:
        std::cout << "Received KNOWN message" << std::endl;
        dataTxt = io.readFile(filenameK);
        io.playBuzzer(dataTxt);
        break;

    case io.UNKOWN:
        std::cout << "Received UNKNOW message" << std::endl;
        dataTxt = io.readFile(filenameU);
        io.playBuzzer(dataTxt);
        break;

    default:
        std::cerr << "Message is wrong!" << std::endl;
        break;
    }

    // Close sockets when done
    close(clientSocket);
    close(serverSocket);

    return 0;
}
