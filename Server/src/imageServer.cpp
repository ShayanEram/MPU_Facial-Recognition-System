/**
 * @file imageServer.cpp
 * @brief C++ program for the server side
 * @author Shayan Eram
 *
 * This code is a TCP server that accepts image data from clients and sends compressed images over the network.
 */
// Include all defines and common values and functions
#include "serverFuncs.hpp"
#include "Connector.hpp"
#include "InputOutput.hpp"
#include "videoManager.hpp"

/**
 * @brief Main function for the server.
 *
 * This function initializes variables, establishes a connection with the client, and handles image capture and communication.
 */
int main(int argc, char const* argv[])
{
    Connector& connect = Connector::getInstance();
    InputOutput& io = InputOutput::getInstance();
    VideoManager& video = VideoManager::getInstance();
    
    // Initialize variables
    uint32_t messages {0x00000000};
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt {1};
    socklen_t addrlen = sizeof(address);
    int imageSize {3326};
    bool connect_OK {true};
    int index {0};
    bool light {false};
    bool btn {false};
    int testRes {0};
    bool btnUsed {false};

    // Connect to client
    connect.serverConnect(server_fd, address, opt);
    cout << "Socket Bound to port: " << PORT << endl;
    
    while(true)
    {
        // Accept client
        connect.serverAccept(server_fd, new_socket, address, addrlen, connect_OK);

        // Prepare the image Capture
        VideoCapture capture(0);
        capture.set(CV_CAP_PROP_FRAME_WIDTH, image[index].resX);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, image[index].resY);
        
        while(connect_OK)
        {
            // Light and btn check
            light = io.readADC();
            btn = io.readGPIO();
            cout << "Light is: " << light << endl;
            cout << "btn is: " << btn << endl;
            
            // Send messages to client
            connect.sendMessage(messages, new_socket, light, btn, btnUsed, capture);
            
            // Run serverExec file
            if(light == true && btn == true && btnUsed == false)
            {
                int error_exec;
                pid_t pid = fork();
            
                if (pid == -1) 
                {
                    perror("fork failed");
                    exit(EXIT_FAILURE);
                } 
                else if (pid == 0) 
                {
                    char * argv_list[] = {NULL};
                    error_exec = execv("/home/root/bin/serverExec",argv_list);
                    if (error_exec == -1) {
                        perror("execv failed");
                        _exit(EXIT_FAILURE);
                    }
                }
                return EXIT_SUCCESS;
            }

            // Read messages from client
            connect.readClient(messages, new_socket, connect_OK, index, testRes, valread, capture);
        }
        // Close connection
        close(new_socket);
        cout << "Session with client finished" << endl;
    }
    // Closing the listening socket
    close(server_fd);
    return 0;
}