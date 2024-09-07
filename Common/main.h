/**
 * @mainpage TCP Image Transfer Project
 *
 * @section intro Introduction
 *
 * This project involves a TCP server and client for transferring images over a network.
 * The server captures images from a camera and sends them to the client, which can display the images
 * and perform face recognition.
 *
 * @section structure Project Structure
 *
 * The project consists of two main components:
 * - TCP Server: Responsible for capturing images and sending them to clients.
 * - TCP Client: Receives images from the server and can perform face recognition.
 *
 * @section dependencies Dependencies
 *
 * The project relies on the following libraries:
 * - OpenCV: For image processing and capturing.
 *
 * @section installation Installation Instructions
 *
 * To install and set up the project, follow these steps:
 * 1. Install OpenCV library on your system.
 * 2. Clone the project repository from GitHub.
 * 3. Compile the server and client source files using a C++ compiler.
 *
 * @section usage Usage
 *
 * To use the project, follow these steps:
 * 1. Start the TCP server by running the server executable.
 * 2. Run the TCP client and connect it to the server.
 * 3. The client will receive images from the server and display them.
 * 4. Optionally, perform face recognition on the received images.
 *
 * @section examples Examples
 *
 * Here's an example code snippet demonstrating how to use the TCP server:
 * @code
 * // Start the TCP server
 * ./server
 * @endcode
 *
 * @section license License
 *
 * This project is licensed under my License.
 */
