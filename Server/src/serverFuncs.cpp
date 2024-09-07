/**
 * @file serverFuncs.h
 * @brief src file for server-side functions used in the program.
 * @author Shayan Eram
 */
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <vector>

// Include all defines and commun values
#include "serverFuncs.h"
#include "../commun/imageCommun.h"

using namespace std;
using namespace cv;


struct Resolution
{
    int resX;   /**< Resolution width. */
    int resY;   /**< Resolution height. */
    double fps; /**< Frames per second. */
};


vector<Resolution> image = 
{
    {176, 144, 0},
    {160, 120, 0},
    {320, 176, 0},
    {320, 240, 0},
    {352, 288, 0},
    {432, 240, 0},
    {800, 600, 0},
    {864, 480, 0},
    {960, 544, 0},
    {960, 720, 0},
    {1184, 656, 0},
    {1280, 720, 0},
    {1280, 960, 0}
};


int CalculateFPS()
{
    for (int i = 0; i < 13; i++)
    {
        // Open the video capture device
        VideoCapture capture(0);

        // Set the resolution for the capture device
        capture.set(CV_CAP_PROP_FRAME_WIDTH, image[i].resX);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, image[i].resY);

        if (!capture.isOpened()) {
            cout << "Failed to connect to the camera." << endl;
        }

        // Declare Mat objects to store the captured frame and the detected edges
        Mat frame, edges;
        capture >> frame;
        capture >> frame;

        // Measure the time it takes to process a certain number of frames
        struct timespec start, end;
        clock_gettime(CLOCK_REALTIME, &start);

        int frames = 10;

        for (int j = 0; j < frames; j++) {
            capture >> frame;
            if (frame.empty()) {
                cout << "Failed to capture an image" << endl;
                return -1;
            }
            // Convert the captured color image to grayscale
            cvtColor(frame, edges, CV_BGR2GRAY);
            // Apply the Canny edge detection algorithm to the grayscale image
            Canny(edges, edges, 0, 30, 3);
        }

        // Record the end time
        clock_gettime(CLOCK_REALTIME, &end);
        // Calculate the time taken to process the frames and convert it to seconds
        double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;

        // Calculate frames per second and store it
        image[i].fps = frames / difference;

        cout << "Resolution: " << image[i].resX << "x" << image[i].resY << " FPS: " << image[i].fps << endl;
        imwrite("edges.png", edges);
        imwrite("capture.png", frame);
    }
    return 0;
}


int PromptSelectResolution()
{
    int selectResX, selectResY;
    double selectFPS = -1;

    cout << "Select Image Width:";
    cin >> selectResX;
    cout << endl << "Select Image Height:";
    cin >> selectResY;
    
    // Find the selected resolution in the predefined resolutions
    for (int i = 0; i < image.size(); i++) {
        if (image[i].resX == selectResX && image[i].resY == selectResY) {
            selectFPS = image[i].fps;
            return i;
        }
    }
    return -1;
}


int capture() 
{
    CalculateFPS();
    int index = PromptSelectResolution();

    while (index == -1) {
        index = PromptSelectResolution();
        cout << endl;
    }

    cout << "Selected Resolution is: " << image[index].resX << "x" << image[index].resY << " at " << image[index].fps << " FPS" << endl;

    VideoCapture capture(0);

    if (!capture.isOpened()) {
        cout << "Failed to connect to the camera." << endl;
    }

    capture.set(CV_CAP_PROP_FRAME_WIDTH, image[index].resX);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, image[index].resY);
    capture.set(CV_CAP_PROP_FPS, image[index].fps);

    // Create a VideoWriter to save the captured video
    VideoWriter vid("capture-liv1.avi", CV_FOURCC('M', 'J', 'P', 'G'), image[index].fps, cv::Size(image[index].resX, image[index].resY), true);

    if (!vid.isOpened()) {
        std::cout << "Error: Unable to open VideoWriter." << std::endl;
        return 1;
    }

    // Record for 5 seconds
    int nFrames = int(5.0 * image[index].fps);
    Mat frame;

    // Capture and write frames to the video file
    for (int i = 0; i < nFrames; i++) {
        if (capture.read(frame)) {
            vid.write(frame);
        }
        else if (frame.empty()) {
            cout << "Failed to capture an image" << endl;
            return -1;
        }
    }

    vid.release();
    capture.release();
    cout << "Video recording complete. Saved as capture-liv1.avi";

    return 0;
}


void captureSendImage(int new_socket, VideoCapture& capture)
{
	vector<uchar> encode(1);
	 		
	if(!capture.isOpened()) {
		cout << "Failed to connect to the camera." << endl;
	}

	Mat frame;
	if(!capture.read(frame)) {	
		cout<< "Capture frame failed"<<endl;
		//return -1;
	}
	imencode(".JPEG",frame,encode);
	int imageSize = encode.size();
	
	//Send vector size
	send(new_socket, &imageSize, sizeof(imageSize), 0);
	
	//Send compressed JPEG image
	send(new_socket,encode.data(),encode.size(),0);
}


void sendMessage(uint32_t& messages, int& new_socket, bool& light, bool& btn, bool& btnUsed, VideoCapture& capture)
{
	//send light and btn messages
	if (light == false){
		messages = IDOWN;
		send(new_socket, &messages, sizeof(messages), 0);
		cout << "Messages sent: light false: "<< messages << endl;
		btnUsed = false;
	}
	else if(light == true && btn == false){
		messages = READY;
		send(new_socket, &messages, sizeof(messages), 0);
		cout << "Messages sent: light true but btn false: " << messages << endl;
		btnUsed = false;
		captureSendImage(new_socket, capture);
	}
	else if(light == true && btn == true && btnUsed == false){
		messages = PUSHB;
		send(new_socket, &messages, sizeof(messages), 0);
		cout << "Messages sent: light true and btn true: " << messages << endl;
		btnUsed = true,
		captureSendImage(new_socket, capture);
	}
	else if(light == true && btn == true && btnUsed == true){
		messages = READY;
		send(new_socket, &messages, sizeof(messages), 0);
		cout << "Messages sent: light true but btn forced false: " << messages << endl;
		btnUsed = true;
		captureSendImage(new_socket, capture);
	}
}


void readClient(uint32_t& messages, int& new_socket, bool& connect_OK, int& index, int& testRes, ssize_t& valread, VideoCapture& capture)
{
	// Read messages from the client for OK or Quit and resolutions
	valread = read(new_socket,&messages,sizeof(messages));

	// Select resolution
	if(messages == RES1_OK){
		index = 1;
		connect_OK = true; // ok to continue

		if(index != testRes){
			capture.set(CV_CAP_PROP_FRAME_WIDTH, image[index].resX);
			capture.set(CV_CAP_PROP_FRAME_HEIGHT, image[index].resY);
			testRes = 1;
			cout << "Res 1 set"<<endl;
		}

	}
	else if(messages == RES2_OK){
		index = 4;
		connect_OK = true; // ok to continue

		if(index != testRes){
			capture.set(CV_CAP_PROP_FRAME_WIDTH, image[index].resX);
			capture.set(CV_CAP_PROP_FRAME_HEIGHT, image[index].resY);
			testRes = 4;
			cout<<"Res 2 rest"<<endl;
		}

	}
	else if(messages == OK){
		connect_OK = true; // ok to continue
	}
	else{
		connect_OK = false; //quit to quit
	}
}


bool readADC()
{
	std::string lightPath = "/sys/class/saradc/ch0";
	std::string lightValue;

	ifstream lightFile;
	lightFile.open(lightPath.c_str());

	if(lightFile.fail()){
		cout << "Failed to open lightFile";
		exit(-1);
	}

	lightFile >> lightValue;
	cout << lightValue << endl;

	lightFile.close();
	
	//inverse logique
	if (stoi(lightValue) < 1000)
		return true;
	else
		return false;
}


bool readGPIO()
{
	std::string exportPath = "/sys/class/gpio/export";
	std::string directionPath = "/sys/class/gpio/gpio228/direction";
	std::string btnPath = "/sys/class/gpio/gpio228/value";

	std::string btnValue;

	std::ofstream exportFile;
	std::ofstream directionFile;
	std::ifstream btnFile;

	exportFile.open(exportPath.c_str(),std::ofstream::out | std::ios::trunc);
	directionFile.open(directionPath.c_str(),std::ofstream::out | std::ios::trunc);

	exportFile << "228";
	directionFile << "in";

	exportFile.close();
	directionFile.close();

	btnFile.open(btnPath.c_str());
	if(btnFile.fail()){
		cout << "failed to open btnFile" << endl;
		exit(-1);
	}

	btnFile >> btnValue;

	btnFile.close();

	// inverse logique
	if(stoi(btnValue) == 0)
		return true;
	else
		return false;
}


void serverAccept(int& server_fd, int& new_socket, struct sockaddr_in& address, socklen_t& addrlen, bool& connect_OK)
{
	// Start listening for incoming connections
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// Accept incoming connections
	if ((new_socket= accept(server_fd, (struct sockaddr*)&address,&addrlen))< 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	cout<<"Accepted Client Connection:"<<endl;
	connect_OK = true;
}


int serverConnect(int& server_fd, struct sockaddr_in& address, int& opt)
{
	// Creating socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Attaching socket to the port PORT
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	// Configure the server address
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Bind the socket to the specified port
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}