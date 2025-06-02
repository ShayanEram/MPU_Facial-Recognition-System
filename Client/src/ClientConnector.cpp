#include "ClientConnector.hpp"

ClientConnector& ClientConnector::getInstance() {
    static ClientConnector instance;
    return instance;
}

int ClientConnector::clientConnect(int& client_fd,struct sockaddr_in& serverAddr, int& status)
{
    // Create a socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "192.168.7.2", &serverAddr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // Connect to the server
    if ((status = connect(client_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))) < 0) {
        perror("connection failed");
        return -1;
    }
}

void ClientConnector::keyPress(int& waitK, uint32_t& messages, int& client_fd)
{
    
    cout << "....................." << endl;
    cout << "Press ESC for quit"<<endl;
    cout << "Press 1 for res1"<<endl;
    cout << "Press 2 for res2"<<endl;
    cout << "Press 5 for Mode apprentissage"<<endl;
    cout << "Press 6 for Mode reconnaissance" << endl;
    cout << "....................." << endl;
    
    if (waitK == ESC) {
        // Set messages to Quit and send to server
        messages = QUIT;    // Force messages to 0x0
        
        send(client_fd, &messages, sizeof(messages), 0);
        close(client_fd);
        cout << "Closing connection" << endl;

    } 
    else if(waitK == K1){
        // set messages to res1 and ok
        messages = RESO1;
        messages |= OK;
    }
    else if(waitK == K2){
        // set messages to res2 and ok
        messages = RESO2;
        messages |= OK;
    }
    else {
        // set messages to OK
        messages = OK;
    }
    // Send to server
    send(client_fd, &messages, sizeof(messages), 0);

}

Mat ClientConnector::recivImage(int& valread, int& client_fd, int& vectorSize)
{
    valread = read(client_fd, &vectorSize, sizeof(vectorSize));

    vector<uchar> imageCode(vectorSize);
    size_t bytesRead = 0;
    
    while (bytesRead < vectorSize) {
        ssize_t result = read(client_fd, imageCode.data() + bytesRead, imageCode.size() - bytesRead);
        
        if (result < 0) {
            cout << "Result error" << endl;
            break;
        } 
        else {
            bytesRead += result;
        }
    }
    
    Mat image = imdecode(Mat(imageCode), IMREAD_UNCHANGED);
    
    if (image.empty()) {
        cout << "Failed to decode the image." << endl;

    }

    return image;
}