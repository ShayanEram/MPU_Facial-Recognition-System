#ifndef serverFuncs_H
#define serverFuncs_H
/**
 * @file serverFuncs.h
 * @brief Header file for server-side functions used in the program.
 * @author Shayan Eram
 */
#include <vector>
#include <opencv2/opencv.hpp>

// Include all defines and common values
#include "../common/imageCommon.h"

/**
 * @brief Resolutions used for the camera.
 */
struct Resolution;

/**
 * @brief Vector of Resolution type.
 */
extern vector<Resolution> image;

/**
 * @brief Function to calculate frames per second for different resolutions.
 * This function calculates frames per second (fps) for a range of resolutions.
 * @return 0 on success, -1 on error.
 */
int CalculateFPS();

/**
 * @brief Function to prompt the user to select a resolution.
 * This function prompts the user to select a custom resolution and returns the index of the selected resolution.
 * @return Index of the selected resolution or -1 if not found.
 */
int PromptSelectResolution();

/**
 * @brief Function to capture video.
 * This function captures a video with a resolution selected by the user and saves it to a file.
 * @return 0 on success, 1 on error.
 */
int capture();

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
 * @brief Function to read the light sensor value.
 * This function reads the light sensor value and returns true if the light is on, false otherwise.
 * @return True if the light is on, false otherwise.
 */
bool readADC();

/**
 * @brief Function to read the button input value.
 * This function reads the button input value and returns true if the button is pressed, false otherwise.
 * @return True if the button is pressed, false otherwise.
 */
bool readGPIO();

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

#endif /* SERVERFUNCS_H */