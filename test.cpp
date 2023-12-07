#include <iostream>
#include <fstream>
#include </home/tian14098/Desktop/check_opencv_work/json.hpp>
#include <opencv2/opencv.hpp>

using json = nlohmann::json;

class ROIExtractor {
public:
    cv::Mat extractROI(const cv::Mat& inputImage, const std::string& jsonFilePath, const std::string& roiName) {
        // Parse JSON file to get ROI points
        std::vector<std::vector<cv::Point>> zones = parseJSON(jsonFilePath, roiName);
        std::cout << "Size of zones:" << zones.size() << std::endl;
        // Check if there are enough zones
        if (zones.size() == 0) {
            std::cerr << "Error: No zones found in the JSON file for key '" << roiName << "'." << std::endl;
            return cv::Mat();
        }

        // Create a mask for each zone and combine them
        cv::Mat combinedMask = cv::Mat::zeros(inputImage.size(), CV_8U);
        for (const auto& zone : zones) {
            cv::Mat mask = cv::Mat::zeros(inputImage.size(), CV_8U);
            std::vector<cv::Point> polygon(zone.begin(), zone.end());
            cv::fillConvexPoly(mask, polygon.data(), static_cast<int>(polygon.size()), cv::Scalar(255));
            combinedMask |= mask;
        }

        // Apply the combined mask to the input image to get the ROI
        cv::Mat roi;
        inputImage.copyTo(roi, combinedMask);

        return roi;
    }

private:
    std::vector<std::vector<cv::Point>> parseJSON(const std::string& jsonFilePath, const std::string& roiName) {
        std::ifstream inputFile(jsonFilePath);
        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open JSON file." << std::endl;
            return {};
        }

        json jsonData;
        inputFile >> jsonData;

        std::vector<std::vector<cv::Point>> zones;
        if (jsonData.find(roiName) != jsonData.end()) {
            auto roiData = jsonData[roiName];

            for (auto& colorData : roiData.items()) {
                auto colorPoints = colorData.value();
                std::vector<cv::Point> points;
                for (const auto& point : colorPoints) {
                    int x = point[0];
                    int y = point[1];
                    points.emplace_back(x, y);
                }
                zones.push_back(points);
            }
        }

        return zones;
    }
};

void drawZones(const std::string& jsonFilePath, const std::string& roiName) {
    // Example usage
    cv::Mat img = cv::imread("/home/tian14098/Desktop/check_opencv_work/large_img_test.png");

    if (img.empty()) {
        std::cerr << "Error: Could not read the image." << std::endl;
        return;
    }

    ROIExtractor roiExtractor;

    // Extract ROI using zones from JSON file
    cv::Mat roi = roiExtractor.extractROI(img, jsonFilePath, roiName);

    // Display the original image and the extracted ROI
    cv::imshow("Original Image", img);
    cv::imshow("ROI", roi);
    cv::waitKey(0);
}

int main() {
    // Provide the path to your JSON file
    std::string jsonFilePath = "/home/tian14098/Desktop/check_opencv_work/roi.json";

    // Loop to handle keyboard events
    while (true) {
        // Wait for a key event
        int key = cv::waitKey(0);

        // Exit loop if 'Esc' is pressed
        if (key == 27) {
            break;
        }

        // Convert the pressed key to a string
        std::string roiName = std::to_string(key - '0');

        // Draw zones based on keypress
        drawZones(jsonFilePath, roiName);
    }

    return 0;
}
