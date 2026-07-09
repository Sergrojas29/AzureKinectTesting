#pragma once
#include <k4a/k4a.h>
#include <k4abt.h>
#include <cmath>
#include <algorithm>
#include <print>

template <typename T>
T square(T x)
{
    return x * x;
}

inline float get_Rad_3D(const k4a_float3_t &pointA, const k4a_float3_t &pointB, const k4a_float3_t &vertex)
{
    // Calculate vectors
    float u_x = pointA.xyz.x - vertex.xyz.x;
    float u_y = pointA.xyz.y - vertex.xyz.y;
    float u_z = pointA.xyz.z - vertex.xyz.z;

    float v_x = pointB.xyz.x - vertex.xyz.x;
    float v_y = pointB.xyz.y - vertex.xyz.y;
    float v_z = pointB.xyz.z - vertex.xyz.z;

    // Calculate vector magnitudes
    float u_vec = std::sqrtf(square(u_x) + square(u_y) + square(u_z));
    float v_vec = std::sqrtf(square(v_x) + square(v_y) + square(v_z));

    // Prevent divide by zero
    if (u_vec == 0.0f || v_vec == 0.0f)
    {
        return 0.0f;
    }

    // Manual dot product
    float dot_product = (u_x * v_x) + (u_y * v_y) + (u_z * v_z);

    // Calculate angle with clamp to protect against floating point drift
    float cosTheta = std::clamp(dot_product / (u_vec * v_vec), -1.0f, 1.0f);

    return std::acos(cosTheta);
}

enum class PoseState
{
    POSE_EMPOWERED,
    POSE_MORGAN,
    POSE_CUTE_STANDING,
    POSE_LEAN_LEFT,
    LOOK_LEFT,
    LOOK_RIGHT
};

class PoseChecker
{
public:
    PoseChecker() = delete;
    static bool check(PoseState Pose, const k4abt_skeleton_t &skeleton);

private:
};
