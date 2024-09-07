/**
 * @file main.cpp
 * @author Shayan Eram
 * @brief Server managing messages and controlling the buzzer based on the received message type.
 * 
 */

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

#define LEARN 0x00000040
#define KNOWN 0x00000080
#define UNKOWN 0x000000c0

/**
 * @struct dataBuz
 * @brief Structure representing the data associated with the buzzer.
 */
struct dataBuz
{
    uint32_t frequency; /**< Buzzer frequency */
    uint32_t length;    /**< Buzzer length */
    uint32_t delay;     /**< Delay between buzzers */
};

/**
 * @brief Reads data from the text file and stores it in a vector.
 * 
 * @param filename The name of the text file.
 * @return A vector containing the data from the text file.
 */
std::vector<dataBuz> readFile(std::string filename)
{
    std::vector<dataBuz> dataTxt;

    dataBuz c;

    std::ifstream file(filename.c_str(), std::ifstream::in);
    if (file.fail())
    {
        std::cout << "Failed to open " + filename << std::endl;
        exit(EXIT_FAILURE);
    }
    while (!file.eof())
    {
        std::string data1, data2, data3, data4, data5, frequency, length, delay;

        file >> data1 >> data2 >> data3;
        file.ignore(1);

        file >> data4 >> data5;
        file.ignore(1);

        frequency.assign(data2, 10, 3);
        length.assign(data3, 7, 3);
        delay.assign(data5, 0, 3);

        c.frequency = atoi(frequency.c_str());
        c.delay = atoi(delay.c_str());
        c.length = atoi(length.c_str());

        dataTxt.push_back(c);
    }
    return dataTxt;
}

/**
 * @brief Controls the buzzer based on the given frequency and length.
 * 
 * @param frequency Buzzer frequency.
 * @param length Buzzer length.
 */
void Buzzer(uint32_t frequency, uint32_t length)
{
    std::string frequencyPath = "/sys/devices/pwm-ctrl.42/freq0";
    std::string enablePath = "/sys/devices/pwm-ctrl.42/enable0";

    // Write the file to activate it
    ofstream freqFile;
    freqFile.open(frequencyPath.c_str());
    freqFile << frequency;

    ofstream enableFile;
    enableFile.open(enablePath.c_str());
    enableFile << 1;

    freqFile.close();
    enableFile.close();

    // Play for the given time in ms
    usleep(length * 1000);

    // Write the file to close it
    enableFile.open(enablePath.c_str());
    enableFile << 0;
    enableFile.close();
}

/**
 * @brief Plays the buzzers based on the data stored in the vector.
 * 
 * @param dataTxt Vector containing data from the text file.
 */
void playBuzzer(const std::vector<dataBuz> &dataTxt)
{
    for (const auto &it : dataTxt)
    {
        Buzzer(it.frequency, it.length);
        usleep((it.delay - it.length) * 1000);
    }
}

/**
 * @brief Main function of the serverExec.
 * 
 * @return 0 if execution is successful.
 */
int main()
{
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
    case LEARN:
        std::cout << "Received LEARN message : Nothing to do!" << std::endl;
        break;

    case KNOWN:
        std::cout << "Received KNOWN message" << std::endl;
        dataTxt = readFile(filenameK);
        playBuzzer(dataTxt);
        break;

    case UNKOWN:
        std::cout << "Received UNKNOW message" << std::endl;
        dataTxt = readFile(filenameU);
        playBuzzer(dataTxt);
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
