cmake_minimum_required(VERSION 3.12)
project(OpenCVWithGPU)

find_package(OpenCV REQUIRED)
find_package(CUDA REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main extract_roi.cpp)

target_link_libraries(main ${OpenCV_LIBS} ${CUDA_LIBRARIES})

# Link with the JSON library using the full path
target_link_libraries(main /home/tian14098/Desktop/check_opencv_work/json.hpp)


# cmake_minimum_required(VERSION 3.10)

# project(OpenCVCamera)

# # Find OpenCV package
# find_package(OpenCV REQUIRED)

# # Add executable
# add_executable(main main.cpp)

# # Link OpenCV libraries
# target_link_libraries(main PRIVATE ${OpenCV_LIBS})
