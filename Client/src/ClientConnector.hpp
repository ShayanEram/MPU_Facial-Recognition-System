#ifndef CLIENTCONNECTOR_H
#define CLIENTCONNECTOR_H

#include "clientFuncs.hpp"

class ClientConnector {
public:

    static ClientConnector& getInstance();

    /**
     * @brief Function to connect to the server.
     *
     * This function creates a socket, sets up the server address, and connects to the server.
     *
     * @param client_fd Reference to the client socket file descriptor.
     * @param serverAddr Reference to the server address structure.
     * @param status Reference to the status variable.
     * @return 0 on success, -1 on failure.
     */
    int clientConnect(int& client_fd, struct sockaddr_in& serverAddr, int& status);

    /**
     * @brief Function to handle key press events.
     *
     * This function handles key press events, sets appropriate messages, and sends them to the server.
     *
     * @param waitK Reference to the variable storing the pressed key.
     * @param messages Reference to the variable storing the messages.
     * @param client_fd Client socket file descriptor.
     */
    void keyPress(int& waitK, uint32_t& messages, int& client_fd);

    /**
     * @brief Function to receive an image from the server.
     *
     * This function receives image data from the server and decodes it using OpenCV.
     *
     * @param valread Reference to the variable storing the number of bytes read.
     * @param client_fd Client socket file descriptor.
     * @param vectorSize Reference to the variable storing the image vector size.
     * @return Decoded image as a Mat object.
     */
    Mat recivImage(int& valread, int& client_fd, int& vectorSize);

private:
    ClientConnector() = default;
    ClientConnector(const ClientConnector&) = delete;
    ClientConnector& operator=(const ClientConnector&) = delete;
};

#endif // CLIENTCONNECTOR_H