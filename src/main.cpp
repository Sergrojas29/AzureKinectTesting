#include "Kinect.h"
#include <print>
#include <chrono>
#include <thread>
#include <exception>
#include <cmath>
#include <numeric>
#include <numbers>

template <typename T>
T square(T x) {
    return x * x;
}


float get_Rad_3D(const k4a_float3_t& positionA, const k4a_float3_t& positionB , const k4a_float3_t vertex){

    const float& verX = vertex.xyz.x;const float& verY = vertex.xyz.y;const float& verZ = vertex.xyz.z;
    const float& aX = positionA.xyz.x;const float& aY = positionA.xyz.y;const float& aZ = positionA.xyz.z;
    const float& bX = positionB.xyz.x;const float& bY = positionB.xyz.y;const float& bZ = positionB.xyz.z;

    std::array<float, 3> u = { verX - bX, verY - bY , verZ - bZ};
    std::array<float, 3> v = { verX - aX, verY - aY , verZ - aZ};

    float u_vec = std::sqrtf( square(u[0]) + square(u[1]) + square(u[2]));
    float v_vec = std::sqrtf( square(v[0]) + square(v[1]) + square(v[2]));

    if (u_vec == 0.0f || v_vec == 0.0f) {
        return 0.0f;
    }


    float dot_product = std::inner_product(u.begin(), u.end(), v.begin(), 0.0f);
    float cosTheta = std::clamp(dot_product / (u_vec * v_vec), -1.0f, 1.0f);

    return std::acos(cosTheta);
}



int main() {


    // try
    // {
    //     Kinect K4aDevice;
    //     std::this_thread::sleep_for(std::chrono::seconds(2));
    //     std::println("Initialization complete. Step in front of the camera!");

    //     int check = 0;
    //     while (check < 30) {
    //         // Poll the camera for the latest head position
    //         auto body = K4aDevice.getBodyPosition();



    //         // std::optional allows us to easily check if data was actually returned
    //         if (body.has_value()) {
                
    //             const auto& detected_body = body.value();



    //             detected_body.skeleton.joints[0].position.xyz;

    //             auto head_pos = detected_body.skeleton.joints[K4ABT_JOINT_HEAD].position;
    //             // auto left_pos = detected_body.skeleton.joints[K4ABT_JOINT_HAND_LEFT].position;
    //             // // Read the 3D (X[0], Y[1], Z[2] in millimeters)
    //             // float x = head_pos.v[0];
    //             // float y = head_pos.v[1];
    //             // float z = head_pos.v[2];


    //             // float LHx = left_pos.v[0];
    //             // float LHy = left_pos.v[1];
    //             // float LHz = left_pos.v[2];

    //             // float res = y - LHy;

    //             // if( res > 0 ){
    //             //     std::println("left Hand is {:.2f}mm above your head", res);
    //             //     // std::println("Head Coordinates -> X: {:.2f}mm, Y: {:.2f}mm, Z: {:.2f}mm", x, y, z);
    //             // }

    //             bool is_tpose = K4aDevice.tPoseChecker(detected_body);
    //             if(is_tpose){
    //                 std::println("tpose is checked and added");
    //                 check++;
    //             }else{
    //                 check = 0;
    //             }


    //         }

    //         // Sleep for ~33ms to run the loop at roughly 30 Frames Per Second (FPS).
    //         // This prevents the while-loop from maxing out your CPU at 100%.
    //         std::this_thread::sleep_for(std::chrono::milliseconds(33));
    //     }

    // }
    // catch(const std::runtime_error& e)
    // {
    //     std::println("CRITICAL ERROR: {}", e.what());
    //     return 1;
    // }

    


    
    return 0;
}