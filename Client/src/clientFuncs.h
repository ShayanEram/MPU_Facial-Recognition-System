#ifndef clientFuncs_H
#define clientFuncs_H
/**
 * @file clientFuncs.h
 * @brief Header file for client functions used in the client-side program.
 * @author Shayan Eram
 */
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>

#include "../commun/imageCommun.h"

using namespace std;
using namespace cv;

/**
 * @brief Patch for the to_string() function.
 */
namespace patch
{
    template <typename T> std::string to_string(const T& n)
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

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
 * @brief Crops the detected face in the image and saves the image.
 *
 * @param image Source image to process and crop.
 * @param imageID Image identifier for saving.
 */
void cropFace(Mat& image, int& imageID);

/**
 * @brief Detects faces in the image and saves the image.
 *
 * @param image Source image to process and detect faces.
 * @param imageID Image identifier for saving.
 */
void detectFace(Mat& image, int& imageID);

/**
 * @brief Normalizes an image to a scale of 0 to 255.
 *
 * @param _src Source image to normalize.
 * @return Normalized image on a scale of 0 to 255.
 */
static Mat norm_0_255(InputArray _src);

/**
 * @brief Reads a CSV file containing image paths and corresponding labels.
 *
 * @param filename CSV file name to read.
 * @param images Vector to store the read images.
 * @param labels Vector to store the read labels.
 * @param separator Separator character in the CSV file (default ';').
 */
static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';');

/**
 * @brief Trains a face recognition model and performs prediction.
 *
 * @param fn_csv CSV file name containing image paths and labels.
 */
void trainAndPredict(const string& fn_csv);

/**
 * @brief Creates a CSV file containing image paths and labels from a base directory.
 *
 * @param base_path Base directory path.
 * @param separator Separator character in the CSV file (default ';').
 */
void createCsv(const string& base_path, const string& separator = ";");

/**
 * @brief Resizes photos in a given directory.
 *
 * @param pathFace Path of the directory containing photos to resize.
 */
void resizePhotos(const string& pathFace);

#endif
