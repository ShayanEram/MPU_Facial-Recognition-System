/**
 * @file Connector.cpp
 * @brief Implementation of the Connector class for handling client-server communication.
 * @author Shayan Eram
 */

#include "Connector.hpp"

static Connector& Connector::getInstance()
{
	static Connector instance;
	return instance;
}

void Connector::captureSendImage(int new_socket, VideoCapture& capture)
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

void Connector::sendMessage(uint32_t& messages, int& new_socket, bool& light, bool& btn, bool& btnUsed, VideoCapture& capture)
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

void Connector::readClient(uint32_t& messages, int& new_socket, bool& connect_OK, int& index, int& testRes, ssize_t& valread, VideoCapture& capture)
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

void Connector::serverAccept(int& server_fd, int& new_socket, struct sockaddr_in& address, socklen_t& addrlen, bool& connect_OK)
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

int Connector::serverConnect(int& server_fd, struct sockaddr_in& address, int& opt)
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