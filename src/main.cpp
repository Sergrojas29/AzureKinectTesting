#include "Kinect.h"
#include "ExportJSON.h"
#include <print>
#include <chrono>
#include <thread>
#include <exception>
#include <cmath>
#include <numeric>
#include <numbers> // For std::numbers::pi
#include <algorithm> 

template <typename T>
T square(T x)
{
    return x * x;
}

float get_Rad_3D(const k4a_float3_t &pointA, const k4a_float3_t &pointB, const k4a_float3_t vertex)
{
    // Vector from vertex to pointA (VA)
    k4a_float3_t vectorVA = {
        vertex.xyz.x - pointA.xyz.x,
        vertex.xyz.y - pointA.xyz.y,
        vertex.xyz.z - pointA.xyz.z
    };

    // Vector from vertex to pointB (VB)
    k4a_float3_t vectorVB = {
        vertex.xyz.x - pointB.xyz.x,
        vertex.xyz.y - pointB.xyz.y,
        vertex.xyz.z - pointB.xyz.z
    };

    // Dot product of VA and VB
    float dot_product = (vectorVA.xyz.x * vectorVB.xyz.x) +
                        (vectorVA.xyz.y * vectorVB.xyz.y) +
                        (vectorVA.xyz.z * vectorVB.xyz.z);

    // Magnitudes of VA and VB
    float magnitudeVA = std::sqrt(square(vectorVA.xyz.x) + square(vectorVA.xyz.y) + square(vectorVA.xyz.z));
    float magnitudeVB = std::sqrt(square(vectorVB.xyz.x) + square(vectorVB.xyz.y) + square(vectorVB.xyz.z));

    // Avoid division by zero
    if (magnitudeVA == 0.0f || magnitudeVB == 0.0f) return 0.0f;

    // Calculate cosine of the angle and clamp to avoid floating point errors outside [-1, 1]
    float cosTheta = std::clamp(dot_product / (magnitudeVA * magnitudeVB), -1.0f, 1.0f);

    return std::acos(cosTheta); // Angle in radians
}

int main()
{

    try
    {
        Kinect K4aDevice;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::println("Initialization complete. Step in front of the camera!");

        // ExportJSON JsonExporter;
        // int poseCaputer = 0;

        int timer = 0;

        while (true)
        {
            // Poll the camera for the latest head position
            auto body = K4aDevice.getBodyPosition();

            // std::optional allows us to easily check if data was actually returned
            if (body.has_value())
            {

                const auto &detected_body = body.value();
                // JsonExporter.setOneFrame(detected_body.skeleton);
                // JsonExporter.WriteToFile("CapPose_"+ std::to_string(poseCaputer)+".json");
                // poseCaputer++;

                const auto &skeleton = detected_body.skeleton.joints;

                float rightLegAngle = get_Rad_3D(skeleton[K4ABT_JOINT_HIP_RIGHT].position, skeleton[K4ABT_JOINT_ANKLE_RIGHT].position, skeleton[K4ABT_JOINT_KNEE_RIGHT].position);
                float leftArmAngle = get_Rad_3D(skeleton[K4ABT_JOINT_SHOULDER_LEFT].position, skeleton[K4ABT_JOINT_WRIST_LEFT].position, skeleton[K4ABT_JOINT_ELBOW_LEFT].position);
                float rightArmAngle = get_Rad_3D(skeleton[K4ABT_JOINT_SHOULDER_RIGHT].position, skeleton[K4ABT_JOINT_WRIST_RIGHT].position, skeleton[K4ABT_JOINT_ELBOW_RIGHT].position);

                float tolerance = .20f; // Radians

                if (std::abs(rightLegAngle - 2.26f) < tolerance &&
                    std::abs(leftArmAngle - 0.87f) < tolerance &&
                    std::abs(rightArmAngle - 0.87f) < tolerance
            )
                {
                    std::cout << "Value is within range." << std::endl;
                    return 1;
                }

                timer++;
            }

            // Sleep for ~33ms to run the loop at roughly 30 Frames Per Second (FPS).
            // This prevents the while-loop from maxing out your CPU at 100%.
            std::this_thread::sleep_for(std::chrono::milliseconds(33));

            // Pause for recalabration
            //  std::this_thread::sleep_for(std::chrono::seconds(10));

            // std::println("Reset pose");
            // std::println("Stand up start");
            // std::println("Lift up right Heel");
            // std::println("Hands to ribs");
            // std::println("Slightly lean ");
        }
    }
    catch (const std::runtime_error &e)
    {
        std::println("CRITICAL ERROR: {}", e.what());
        return 1;
    }

    return 0;
}