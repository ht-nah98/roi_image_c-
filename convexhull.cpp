#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Read an image from file
    cv::Mat image = cv::imread("./images.jpeg");

    // Check if the image was successfully loaded
    if (image.empty()) {
        std::cerr << "Error: Could not open or read the image." << std::endl;
        return -1; // Return an error code
    }

    // Display the image in a window
    cv::imshow("OpenCV Example", image);

    // Wait for a key press and then close the window
    cv::waitKey(0);

    return 0; // Return 0 to indicate successful execution
}
