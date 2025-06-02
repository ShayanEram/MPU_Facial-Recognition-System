/**
 * @file Connector.hpp
 * @brief Header file for the Connector class.
 * This file contains the declaration of the Connector class, which is responsible for managing the connection between the server and client, capturing images from a webcam, and sending messages based on the state of the light and button.
 * @author Shayan Eram
 */
#ifndef CONNECTOR_HPP
#define CONNECTOR_HPP

#include "serverFuncs.hpp"

class Connector
{
    static Connector& getInstance();

    /**
     * @brief Function to capture an image and send it to the client.
     * This function captures an image from the webcam using OpenCV and sends it to the client.
     * @param new_socket The socket for communication with the client.
     * @param capture The OpenCV VideoCapture object for capturing images.
     */
    void captureSendImage(int new_socket, VideoCapture& capture);

    /**
     * @brief Sends a message to the client based on light and button state.
     * This function sends messages to the client based on the state of the light and button.
     * @param messages The message to send.
     * @param new_socket The client socket file descriptor.
     * @param light The state of the light.
     * @param btn The state of the button.
     * @param btnUsed An indicator if the button has already been used.
     * @param capture The OpenCV VideoCapture object for capturing images.
     */
    void sendMessage(uint32_t& messages, int& new_socket, bool& light, bool& btn, bool& btnUsed, VideoCapture& capture);

    /**
     * @brief Reads messages from the client.
     * This function reads messages from the client including OK, Quit, and resolution messages.
     * @param messages The variable to store received messages.
     * @param new_socket The client socket file descriptor.
     * @param connect_OK An indicator whether the connection is successful.
     * @param index The index used for selecting the image resolution.
     * @param testRes The resolution used for testing.
     * @param valread The number of bytes read.
     * @param capture The OpenCV VideoCapture object for capturing images.
     */
    void readClient(uint32_t& messages, int& new_socket, bool& connect_OK, int& index, int& testRes, ssize_t& valread, VideoCapture& capture);

    /**
     * @brief Accepts incoming connections from clients.
     * This function listens for incoming connections and accepts them, establishing communication with the client.
     * @param server_fd The server socket file descriptor.
     * @param new_socket The client socket file descriptor.
     * @param address The server address structure.
     * @param addrlen The size of the server address structure.
     * @param connect_OK An indicator whether the connection is successful.
     */
    void serverAccept(int& server_fd, int& new_socket, struct sockaddr_in& address, socklen_t& addrlen, bool& connect_OK);

    /**
     * @brief Establishes a connection with the server.
     * This function creates a socket, binds it to a specified port, and configures it for listening.
     * @param server_fd The socket file descriptor.
     * @param address The server address structure.
     * @param opt Socket options.
     * @return 0 on success, -1 on failure.
     */
    int serverConnect(int& server_fd, struct sockaddr_in& address, int& opt);
    
private:
    Connector() = default;
    Connector(const Connector&) = delete;
    Connector& operator=(const Connector&) = delete;
};

#endif // CONNECTOR_HPP