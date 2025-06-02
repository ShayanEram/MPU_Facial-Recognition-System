/**
 * @file FaceProcessor.hpp
 * @brief Header file for the FaceProcessor class, which handles face detection, cropping, and image processing.
 * Face recognition code from: https://docs.opencv.org/4.x/da/d60/tutorial_face_main.html
 * @author Shayan Eram
 */

#ifndef FACEPROCESSOR_HPP
#define FACEPROCESSOR_HPP

#include "clientFuncs.hpp"

class FaceProcessor {
public:

    static FaceProcessor& getInstance();

    /**
     * @brief Patch for the to_string() function.
     */
    template <typename T>
    std::string to_string(const T& n);

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

private:
    FaceProcessor() = default;
    ~FaceProcessor() = default;

    FaceProcessor(const FaceProcessor&) = delete;
    FaceProcessor& operator=(const FaceProcessor&) = delete;
};

#endif // FACEPROCESSOR_HPP