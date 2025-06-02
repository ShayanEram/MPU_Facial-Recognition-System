/**
 * @file imageCapture.cpp
 * @brief CPP file to capture webcam video using OpenCV.
 * @author Shayan Eram
 *
 * This program captures video from a webcam at predefined resolutions
 * and calculates frames per second (fps) for each resolution.
 */

#include "videoManager.hpp"

/**
 * @brief Main function for video capture.
 *
 * This function captures video with a resolution selected by the user and saves it to a file.
 * @return 0 on success, 1 on error.
 */
int main() 
{
    VideoManager& video = VideoManager::getInstance();
    
    video.CalculateFPS();
    int index = video.PromptSelectResolution();

    while (index == -1) {
        index = video.PromptSelectResolution();
        cout << endl;
    }

    cout << "Selected Resolution is: " << video.image[index].resX << "x" << video.image[index].resY << " at " << video.image[index].fps << " FPS" << endl;

    VideoCapture capture(0);

    if (!capture.isOpened()) {
        cout << "Failed to connect to the camera." << endl;
    }

    capture.set(CV_CAP_PROP_FRAME_WIDTH, video.image[index].resX);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, video.image[index].resY);
    capture.set(CV_CAP_PROP_FPS, video.image[index].fps);

    // Create a VideoWriter to save the captured video
    VideoWriter vid("capture-liv1.avi", CV_FOURCC('M', 'J', 'P', 'G'), video.image[index].fps, cv::Size(video.image[index].resX, video.image[index].resY), true);

    if (!vid.isOpened()) {
        std::cout << "Error: Unable to open VideoWriter." << std::endl;
        return 1;
    }

    // Record for 5 seconds
    int nFrames = int(5.0 * video.image[index].fps);
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