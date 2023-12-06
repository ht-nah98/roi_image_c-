#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
   cv::VideoCapture cap(0);  // Open camera with index 0

    if (!cap.isOpened()) {
        // Handle error: Unable to open the camera
        return -1;
    }

    // Set the desired frame rate (e.g., 30 fps)
    cap.set(cv::CAP_PROP_FPS, 30);

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the camera." << std::endl;
        return -1;
    }

    cv::Mat frame;

    // Variables for calculating FPS
    int frames = 0;
    auto start = std::chrono::steady_clock::now();

    while (true) {
        // Read a frame from the camera
        auto start_cap = std::chrono::steady_clock::now();
        cap >> frame;
        auto end_cap = std::chrono::steady_clock::now();
        auto total = std::chrono::duration_cast<std::chrono::milliseconds>(end_cap - start_cap).count();
        std::cout << "Capture time: " << total << "\n";
        
        if (frame.empty()) {
            std::cerr << "Error: Could not read a frame from the camera." << std::endl;
            break;
        }

        // Display the frame
        cv::imshow("Camera", frame);

        // Count frames and calculate FPS
        frames++;
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        if (elapsed >= 1000) {
            double fps = frames / (elapsed / 1000.0);
            std::cout << "FPS: " << fps << std::endl;

            // Reset counters
            frames = 0;
            start = std::chrono::steady_clock::now();
        }

        // Break the loop if 'ESC' key is pressed
        if (cv::waitKey(30) == 'q') {
            break;
        }
    }

    return 0;
}
