#ifndef VIDEO_MANAGER_HPP
#define VIDEO_MANAGER_HPP

#include "serverFuncs.hpp"

/**
 * @brief Resolutions used for the camera.
 */
struct Resolution
{
    int resX;   /**< Resolution width. */
    int resY;   /**< Resolution height. */
    double fps; /**< Frames per second. */
};

class VideoManager
{
public:

    static VideoManager& getInstance();

    /**
     * @brief Function to calculate frames per second for different resolutions.
     * This function calculates frames per second (fps) for a range of resolutions.
     * @return 0 on success, -1 on error.
     */
    int CalculateFPS();

    /**
     * @brief Function to prompt the user to select a resolution.
     * This function prompts the user to select a custom resolution and returns the index of the selected resolution.
     * @return Index of the selected resolution or -1 if not found.
     */
    int PromptSelectResolution();

    /**
     * @brief Function to capture video.
     * This function captures a video with a resolution selected by the user and saves it to a file.
     * @return 0 on success, 1 on error.
     */
    int capture();

    /**
     * @brief Vector of Resolution type.
     */
    const std::vector<Resolution> image =
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

private:
    VideoManager() = default;
    VideoManager(const VideoManager&) = delete;
    VideoManager& operator=(const VideoManager&) = delete;
};

#endif // VIDEO_MANAGER_HPP