/**
 * @file FaceProcessor.cpp
 * @brief Source file for face processing functions.
 * @author Shayan Eram
 */

#include "FaceProcessor.hpp"

FaceProcessor& FaceProcessor::getInstance() {
    static FaceProcessor instance;
    return instance;
}

template <typename T> 
static std::string to_string(const T& n)
{
    std:: ostringstream stm;
    stm << n;
    return stm.str();
}

void FaceProcessor::cropFace(Mat& image,int& imageID)
{
    CascadeClassifier face_cascade;
    face_cascade.load("haarcascade_frontalface_default.xml");

    // Convert the image to grayscale for face detection
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Detect faces
    vector<Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.3, 5);
    Mat croppedFace;

    // Folder to save the images
    std::string saveFolder = "./images/shayan/";

    // Draw rectangles around the detected faces
    for (int i=0; i<faces.size();i++) {
        //rectangle(image, faces[i], Scalar(255, 0, 0), 2);
        croppedFace = image(faces[i]);
    }
    // Save image with number
    imwrite(saveFolder + "Image"+patch::to_string(imageID)+".png", croppedFace);
}

void FaceProcessor::detectFace(Mat& image,int& imageID)
{
    // Face detection
    CascadeClassifier face_cascade;
    face_cascade.load("haarcascade_frontalface_default.xml");

    // Convert the image to grayscale for face detection
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Detect faces
    vector<Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.3, 5);

    // Draw rectangles around the detected faces
    for (int i=0; i<faces.size();i++) {
        rectangle(image, faces[i], Scalar(255, 0, 0), 2);
    }
    // Save image with number
    imwrite("Image"+patch::to_string(imageID)+".png", image);
}

Mat FaceProcessor::norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

void FaceProcessor::read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    
    cout << "readCSV is doing!" <<endl;

    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        perror ("No valid input file was given");
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

void FaceProcessor::trainAndPredict(const string& fn_csv)
{
    cout << "train_predict is doing!" <<endl;
    // These vectors hold the images and corresponding labels.
    vector<Mat> images;
    vector<int> labels;
    // Read in the data. This can fail if no valid
    // input filename is given.
    try {
        read_csv(fn_csv, images, labels);
    } catch (const cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(images.size() <= 1) {
        perror("This demo needs at least 2 images to work. Please add more images to your data set!");
    }
 
    int height = images[0].rows;

    Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();

    Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
    model->train(images, labels);

    int predictedLabel = -1;
    double confidence = 0.0;
    model->predict(testSample, predictedLabel, confidence);
   
    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
    cout << result_message << endl;
    cout << "Predict is cofidence lvl:" << confidence <<endl;
}

void FaceProcessor::createCsv(const string& base_path, const string& separator = ";") {
    
    cout << "createCSV is doing!" <<endl;
    DIR* dir = opendir(base_path.c_str());
    if (dir == NULL) {
        cerr << "Error opening directory: " << base_path << endl;
        return;
    }

    // Open the output CSV file
    ofstream csvFile("CSV_file.csv");

    if (!csvFile.is_open()) {
        cerr << "Error opening CSV file: " << "CSV_file.csv" << endl;
        closedir(dir);
        return;
    }

    struct dirent* entry;
    int label = 0;

    while ((entry = readdir(dir)) != NULL) {
        string subdirname = entry->d_name;
        if (subdirname != "." && subdirname != "..") {
            string subject_path = base_path + "/" + subdirname;

            DIR* subdir = opendir(subject_path.c_str());
            if (subdir == NULL) {
                cerr << "Error opening subdirectory: " << subject_path << endl;
                closedir(dir);
                csvFile.close();
                return;
            }

            struct dirent* file;
            while ((file = readdir(subdir)) != NULL) {
                string filename = file->d_name;
                if (filename != "." && filename != "..") {
                    string abs_path = subject_path + "/" + filename;
                    // Write to CSV file
                    csvFile << abs_path << separator << label << endl;
                }
            }
            closedir(subdir);
            label++;
        }
    }
    closedir(dir);
    csvFile.close();
}

void FaceProcessor::resizePhotos(const string& pathFace)
{
    cout << "resizePhotos is doing!" <<endl;
    DIR* subdir = opendir(pathFace.c_str());
    struct dirent* file;

    while ((file = readdir(subdir)) != NULL) {
        string filename = file->d_name;
        if (filename != "." && filename != "..") {
            
            Mat originalImage = imread(pathFace + "/" +filename);

            if (!originalImage.empty()) {
                
                Mat resizedImage;
                resize(originalImage, resizedImage, Size(136, 136));

                imwrite(pathFace + "/" + filename, resizedImage);
            } 
            else 
            {
                cerr << "Error: Image not readble" << endl;
            }
        }
    }
}