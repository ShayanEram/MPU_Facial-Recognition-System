/**
 * @file videoManager.cpp
 * @brief Implementation of the VideoManager class for handling video capture and processing.
 * @author Shayan Eram
 */

#include "videoManager.hpp"

static VideoManager& VideoManager::getInstance()
{
    static VideoManager instance;
    return instance;
}

int VideoManager::CalculateFPS()
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

int VideoManager::PromptSelectResolution()
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

int VideoManager::capture() 
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