# Face Recognition Project

This project involves a TCP server and client for transferring images over a network. The server captures images from a camera and sends them to the client, which can display the images and perform face recognition.

## Requirements

- OpenCV library
- C++ compiler
- Linux environment
- Odroid C2

## Server

The server captures images from a camera and sends them to the connected client. It also monitors the light and button status for intelligent image transmission.

## Client

The client connects to the server, receives images, and displays them. It can also initiate face recognition on received images.

## Face recognition

The client's child process performs face recognition on received images using the OpenCV Haar Cascade Classifier. Detected faces are highlighted, and the images are saved.

### Compilation

```bash
# Server Compilation
g++ -o server imageServer.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc
/server/build
cmake ..
make

# Server Usage
Run the server executable:
./server
The server will start listening for incoming connections and capture images.


# Client Compilation
g++ -o client imageClient.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc
/client/build
cmake ..
make

# Client Usage
Run the client executable:
./client
The client will connect to the server and display received images.
```

## Contributing
No contribution can be made.

## License
This project is licensed under the POLYMT Shayan Eram License.

