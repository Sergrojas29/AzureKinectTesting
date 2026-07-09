#include "Kinect.h"
#include "ExportJSON.h"
#include "PoseChecker.h"
#include <print>
#include <chrono>
#include <thread>
#include <exception>
#include <cmath>
#include <algorithm>
#include <csignal> // For OS signals
#include <atomic>  // For thread-safe booleans
#include <iostream>

// 1. Create a global thread-safe flag
std::atomic<bool> keep_running{true};
std::atomic<bool> Trigger_Pose_Capture{false};
std::atomic<bool> Check_for_Pose{false};

void keyboard_listner()
{
    char input;
    while (keep_running)
    {
        std::cin >> input;
        if (input == 'q')
        {
            Trigger_Pose_Capture = true;
        }
        if (input == 'e')
        {
            Check_for_Pose = true;
        }
        if (input == 'x')
        {
            keep_running = false;
        }
    }
}

int main()
{
    std::thread input_thread(keyboard_listner);

    std::println("Kinect starting...");
    std::println("Type 'q' and press Enter to trigger a pose capture.");
    std::println("Type 'e' and press Enter to Enter Pose Checker and Again to Exit.");
    std::println("Type 'x' and press Enter to quit Application.");

    try
    {
        Kinect K4aDevice;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::println("Initialization complete. Step in front of the camera!");

        // For Export To Json and Counter
        ExportJSON JsonExporter;
        int poseCaputer = 0;

        while (keep_running)
        {
            // Poll the camera for the latest head position
            auto body = K4aDevice.getBodyPosition();

            // std::optional allows us to easily check if data was actually returned
            if (body.has_value())
            {

                const auto &detected_body = body.value();

                if (Trigger_Pose_Capture)
                {
                    JsonExporter.setOneFrame(detected_body.skeleton);
                    JsonExporter.WriteToFile("CapPose_" + std::to_string(poseCaputer) + ".json");
                    poseCaputer++;
                    std::println("POSE TAKEN ______________________________________________");
                    Trigger_Pose_Capture = false;
                }

                if (Check_for_Pose)
                {

                    if (PoseChecker::check(PoseState::POSE_MORGAN, detected_body.skeleton))
                    {
                        std::println("Morgan Pose");
                    }

                    if (PoseChecker::check(PoseState::POSE_EMPOWERED, detected_body.skeleton))
                    {
                        std::println("Empowered Pose");
                    }
                    if (PoseChecker::check(PoseState::POSE_CUTE_STANDING, detected_body.skeleton))
                    {
                        std::println("Cute Pose");
                    }
                    if (PoseChecker::check(PoseState::POSE_LEAN_LEFT, detected_body.skeleton))
                    {
                        std::println("Lean Left Pose");
                    }
                }

            }

            // Sleep for ~33ms to run the loop at roughly 30 Frames Per Second (FPS).
            // This prevents the while-loop from maxing out your CPU at 100%.
            std::this_thread::sleep_for(std::chrono::milliseconds(33));

            // Pause for recalabration
            // std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
    catch (const std::runtime_error &e)
    {
        std::println("CRITICAL ERROR: {}", e.what());
        return 1;
    }

    return 0;
}