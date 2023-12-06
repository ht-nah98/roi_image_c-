#include <opencv2/opencv.hpp>
#include <iostream>
// #include <extract_roi.cpp>

int main() {
    // Example usage
    cv::Mat img = cv::imread("/home/tian14098/Desktop/check_opencv_work/extract_roi.cpp");

    if (img.empty()) {
        std::cerr << "Error: Could not read the image." << std::endl;
        return -1;
    }

    ROIExtractor roiExtractor;

    // Provide the path to your JSON file and the ROI name
    std::string jsonFilePath = "/home/tian14098/Desktop/check_opencv_work/roi.json";
    std::string roiName = "zone1";

    // Extract ROI using points from JSON file
    cv::Mat roi = roiExtractor.extractROI(img, jsonFilePath, roiName);

    // Display the original image and the extracted ROI
    cv::imshow("Original Image", img);
    cv::imshow("ROI", roi);
    cv::waitKey(0);

    return 0;
}