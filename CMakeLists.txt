cmake_minimum_required(VERSION 3.20)

project(KinectProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Azure Kinect Sensor SDK
find_package(k4a REQUIRED)
find_package(k4abt REQUIRED)

include_directories(include)

add_executable(kinect_app 
    src/main.cpp 
    src/Kinect.cpp
)

# Link the Azure Kinect library to your executable
target_link_libraries(kinect_app PRIVATE k4a::k4a k4abt::k4abt)