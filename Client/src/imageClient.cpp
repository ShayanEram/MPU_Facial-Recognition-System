/**
 * @file imageClient.cpp
 * @brief C++ client-side program for receiving image transfer
 * @author Shayan Eram
 */
#include "clientFuncs.hpp"
#include "../commun/imageCommun.hpp"

#include "ClientConnector.hpp"
#include "FaceProcessor.hpp"

/**
 * @brief Main function for the client.
 *
 * This function initializes variables, connects to the server, and handles image reception and key events.
 */
int main(int argc, char const* argv[]) {
    
    ClientConnector& connect = ClientConnector::getInstance();
    FaceProcessor& process = FaceProcessor::getInstance();

    uint32_t messages = 0x00000000;
    int status, valread, client_fd;
    struct sockaddr_in serverAddr;
    int vectorSize;
    int messagesRecived;
    int imageID = 0;
    int modKey = -1;
    const std::string imagePath = "./images";

    // Connect to server
    connect.clientConnect(client_fd, serverAddr, status);
    cout << "Connected to server" << endl;
    
    // Receive image while connected
    while (true) {

        // Read the button
        int waitK = waitKey(DELAY);

        if(waitK != -1)
            modKey = waitK;

        // Receive image from server
        recv(client_fd, &messages, sizeof(messages), 0);

        messagesRecived = messages;
        
        if(messagesRecived == IDOWN){
            cout << "No image to show" << endl;
        }
        else if(messagesRecived == READY){
            Mat image = connect.recivImage(valread, client_fd, vectorSize);
            imshow("Image", image);
        }
        else if(messagesRecived == PUSHB){
            
            Mat image = connect.recivImage(valread, client_fd, vectorSize);
            imageID++;

            pid_t pid = fork();
            
            if(pid == -1)
            {
                perror("fork");
                return -1;
            }
            else if(pid > 0)
            {
                // Parent process
                // cout << "Parent process" << endl;
                imshow("Image", image);
            }
            else
            {
                // Child process
                // cout << "Child process" << endl;
                if(modKey == K5)
                {
                    cout << "Mode: Apprentissage" << endl;
                    // Save cropped faces
                    process.cropFace(image, imageID);
                }
                else if(modKey == K6)
                {
                    cout << "Mode: Reconnaisance" << endl;
                    process.resizePhotos("./images/shayan/");
                    process.createCsv(imagePath);
                    process.trainAndPredict("./CSV_file.csv");
                }
                else
                {
                    // Save detected faces
                    process.detectFace(image, imageID);
                    exit(0);
                }
                exit(0);
            }
        }
        
        // Read the key
        connect.keyPress(waitK, messages, client_fd);
        
        if (waitK == ESC)
            return 0;
    }
    // Closing the connected socket
    return 0;
}
