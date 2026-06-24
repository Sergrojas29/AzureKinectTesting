#pragma once
#include <k4a/k4a.h>
#include <k4abt.h>

// Helper to tell unique_ptr how to safely free k4a device
struct K4aDeviceDeleter
{
    void operator()(_k4a_device_t* device) const {
        //Check for nullptr (typical for all Deleters)
        if (device) {
            k4a_device_stop_cameras(device);
            k4a_device_close(device);
        }
    }
};

// Helper shut down and destroy the AI tracker
struct K4aTrackerDeleter
{
    void operator()(_k4abt_tracker_t* tracker) const {
        if (tracker) {
            k4abt_tracker_shutdown(tracker);
            k4abt_tracker_destroy(tracker);
        }
    }
};

// release raw sensor captures
struct K4aSensorCaptureDeleter
{
    void operator()(_k4a_capture_t* sensor_capture) const {
        if (sensor_capture) {
            k4a_capture_release(sensor_capture);
        }
    }
};

// release processed body tracking frames
struct K4aBodyFrameDeleter
{
    void operator()(_k4abt_frame_t* body_frame) const {
        if (body_frame) {
            k4abt_frame_release(body_frame);
        }
    }
};