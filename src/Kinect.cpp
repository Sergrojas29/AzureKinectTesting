#include "Kinect.h"
#include <print>
#include <stdexcept>

Kinect::Kinect(){
    // Open the physical device
    k4a_device_t raw_device = nullptr;
    if (k4a_device_open(K4A_DEVICE_DEFAULT, &raw_device) != K4A_RESULT_SUCCEEDED) {
        throw std::runtime_error("Error: Failed to open Azure Kinect device.");
    }
    
    m_device.reset(raw_device);
    std::println("Camera opened successfully.");

    // Configure and start the camera
    // Body tracking requires the depth camera to be running
    k4a_device_configuration_t device_config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    device_config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED; // Narrow field of view, unbinned resolution
    device_config.color_resolution = K4A_COLOR_RESOLUTION_OFF; // Turning color off to save bandwidth/VRAM for testing
    if (k4a_device_start_cameras(m_device.get(), &device_config) != K4A_RESULT_SUCCEEDED) {
        throw std::runtime_error("Error: Failed to start cameras.");
    }


    // Get the camera's structural calibration data
    // The tracker needs this geometry map to accurately calculate 3D points
    k4a_calibration_t calibration;
    if (k4a_device_get_calibration(m_device.get(), device_config.depth_mode, device_config.color_resolution, &calibration) != K4A_RESULT_SUCCEEDED) {
        throw std::runtime_error("Error: Failed to get device calibration.");
    }

    // 4. Initialize the Tracker
    k4abt_tracker_t raw_tracker = nullptr;
    k4abt_tracker_configuration_t tracker_config = K4ABT_TRACKER_CONFIG_DEFAULT;
    tracker_config.processing_mode = K4ABT_TRACKER_PROCESSING_MODE_GPU_CUDA; 
    
    if (k4abt_tracker_create(&calibration, tracker_config, &raw_tracker) != K4A_RESULT_SUCCEEDED) {
        throw std::runtime_error("Error: Failed to initialize AI tracker.");
    }
    m_tracker.reset(raw_tracker);    
}


// Polling function demonstrating transient pointers
std::optional<k4a_float3_t> Kinect::getHeadPosition() {
    k4a_capture_t raw_capture = nullptr;
    const int timeout_ms = 0; // 0 timeout means "return immediately if no frame is ready"

    if (k4a_device_get_capture(m_device.get(), &raw_capture, timeout_ms) == K4A_WAIT_RESULT_SUCCEEDED) {
        // Create transient smart pointer. It will die when the function ends.
        unique_capture capture(raw_capture);
        
        if (k4abt_tracker_enqueue_capture(m_tracker.get(), capture.get(), timeout_ms) == K4A_WAIT_RESULT_SUCCEEDED) {
            
            k4abt_frame_t raw_frame = nullptr;
            if (k4abt_tracker_pop_result(m_tracker.get(), &raw_frame, timeout_ms) == K4A_WAIT_RESULT_SUCCEEDED) {
                // Create transient smart pointer
                unique_frame frame(raw_frame);
                
                if (k4abt_frame_get_num_bodies(frame.get()) > 0) {
                    k4abt_body_t body;
                    k4abt_frame_get_body_skeleton(frame.get(), 0, &body.skeleton);
                    
                    // Return the 3D position of the head
                    return body.skeleton.joints[K4ABT_JOINT_HEAD].position;
                }
            }
        }
    }
    
    // Return empty if no frame or no body was detected
    return std::nullopt; 
}