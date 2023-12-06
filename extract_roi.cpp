#include <iostream>
#include <fstream>
#include </home/tian14098/Desktop/check_opencv_work/json.hpp>
// #include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>

using json = nlohmann::json;

class ROIExtractor {
public:
    cv::Mat extractROI(const cv::Mat& inputImage, const std::string& jsonFilePath, const std::string& roiName) {
    // Parse JSON file to get ROI points
    std::vector<cv::Point> roiPoints = parseJSON(jsonFilePath, roiName);
    std::cout<< roiPoints << std::endl;
    // Check if there are enough points
    if (roiPoints.size() < 3) {
        std::cerr << "Error: Not enough points in the JSON file for ROI '" << roiName << "'." << std::endl;
        return cv::Mat();
    }

    // Create a vector of points for the polygon
    std::vector<cv::Point> polygon;
    for (const auto& point : roiPoints) {
        polygon.push_back(point);
    }

    // Check if the polygon has enough points
    if (polygon.size() < 3) {
        std::cerr << "Error: Not enough points in the ROI polygon." << std::endl;
        return cv::Mat();
    }

    // Create a mask for the polygon
    cv::Mat mask = cv::Mat::zeros(inputImage.size(), CV_8U);
    cv::fillConvexPoly(mask, polygon.data(), static_cast<int>(polygon.size()), cv::Scalar(255));

    // Apply the mask to the input image to get the ROI
    cv::Mat roi;
    inputImage.copyTo(roi, mask);

    // Check if the roi matrix is empty
    if (roi.empty()) {
        std::cerr << "Error: Failed to create ROI matrix." << std::endl;
        return cv::Mat();
    }

    return roi;
}

private:
    std::vector<cv::Point> parseJSON(const std::string& jsonFilePath, const std::string& roiName) {
        std::ifstream inputFile(jsonFilePath);
        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open JSON file." << std::endl;
            return {};
        }

        json jsonData;
        inputFile >> jsonData;

        std::vector<cv::Point> points;
        if (jsonData.find(roiName) != jsonData.end()) {
            for (const auto& point : jsonData[roiName]) {
                int x = point[0];
                int y = point[1];
                points.emplace_back(x, y);
            }
        }

        return points;
    }
};


int main() {
    // Example usage
    cv::Mat img = cv::imread("/home/tian14098/Desktop/check_opencv_work/large_img_test.png");

    if (img.empty()) {
        std::cerr << "Error: Could not read the image." << std::endl;
        return -1;
    }

    ROIExtractor roiExtractor;

    // Provide the path to your JSON file and the ROI name
    std::string jsonFilePath = "/home/tian14098/Desktop/check_opencv_work/roi.json";
    std::string roiName = "zone2";

    // Extract ROI using points from JSON file
    cv::Mat roi = roiExtractor.extractROI(img, jsonFilePath, roiName);

    // // Display the original image and the extracted ROI
    cv::imshow("Original Image", img);
    cv::imshow("ROI", roi);
    cv::waitKey(0);

    return 0;
}