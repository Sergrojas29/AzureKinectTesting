#include "Kinect.h"
#include <print>
#include <chrono>
#include <thread>
#include <exception>

int main() {

    try
    {
        Kinect K4aDevice;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::println("Initialization complete. Step in front of the camera!");

    }
    catch(const std::runtime_error& e)
    {
        std::println("CRITICAL ERROR: {}", e.what());
        return 1;
    }

    


    
    return 0;
}