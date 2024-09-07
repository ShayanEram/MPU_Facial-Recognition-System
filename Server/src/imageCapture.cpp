/**
 * @file imageCapture.cpp
 * @brief CPP file to capture webcam video using OpenCV.
 * @author Shayan Eram
 *
 * This program captures video from a webcam at predefined resolutions
 * and calculates frames per second (fps) for each resolution.
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <vector>

using namespace std;
using namespace cv;

// Structure to store resolution and fps information
struct Resolution
{
    int resX; /**< Resolution width */
    int resY; /**< Resolution height */
    double fps; /**< Frames per second */
};

// Vector to store predefined image resolutions
std::vector<Resolution> image = 
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

/**
 * @brief Function to calculate frames per second for different resolutions.
 *
 * This function calculates fps for a range of resolutions.
 * @return 0 on success, -1 on error.
 */
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

/**
 * @brief Function to prompt the user to select a resolution.
 *
 * This function prompts the user to select a custom resolution and returns the index of the selected resolution.
 * @return Index of the selected resolution or -1 if not found.
 */
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

/**
 * @brief Main function for video capture.
 *
 * This function captures video with a resolution selected by the user and saves it to a file.
 * @return 0 on success, 1 on error.
 */
int main() 
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