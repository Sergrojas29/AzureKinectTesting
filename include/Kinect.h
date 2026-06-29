#pragma once
#include "UniquePointerDeleter.h"
#include <k4a/k4a.h>
#include <k4abt.h>
#include <memory>
#include <optional>
 

using unique_device = std::unique_ptr<_k4a_device_t, K4aDeviceDeleter>;
using unique_tracker = std::unique_ptr<_k4abt_tracker_t, K4aTrackerDeleter>;
using unique_capture = std::unique_ptr<_k4a_capture_t, K4aSensorCaptureDeleter>;
using unique_frame = std::unique_ptr<_k4abt_frame_t, K4aBodyFrameDeleter>;


class Kinect
{
private:
    // NEED for initialization of hardware
    unique_device m_device;
    unique_tracker m_tracker;

    // unique_capture m_capture;
    // unique_frame m_frame;

public:
    Kinect();
    ~Kinect() = default;


    // A method to poll the camera and return coordinates
    // Using std::optional in case no body is detected on this specific frame
    std::optional<k4abt_body_t> getBodyPosition();

    bool tPoseChecker(const k4abt_body_t& body);

    

};

