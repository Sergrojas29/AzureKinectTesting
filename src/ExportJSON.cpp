#include "ExportJSON.h"


void ExportJSON::WriteToFile(const crow::json::wvalue& data){

    std::ofstream outFile("data.json");
    if (outFile.is_open()) {
        outFile << data.dump(2) << std::endl;
        outFile.close();
        std::cout << "Successfully wrote clean JSON using Crow!" << std::endl;
    } else {
        std::cerr << "Failed to open data.json for writing." << std::endl;
    }
}


void ExportJSON::getOneFrame(const k4abt_skeleton_t& skeleton){

    std::array<std::string, 32> jointNames = {
        "K4ABT_JOINT_PELVIS",
        "K4ABT_JOINT_SPINE_NAVEL",
        "K4ABT_JOINT_SPINE_CHEST",
        "K4ABT_JOINT_NECK",
        "K4ABT_JOINT_CLAVICLE_LEFT",
        "K4ABT_JOINT_SHOULDER_LEFT",
        "K4ABT_JOINT_ELBOW_LEFT",
        "K4ABT_JOINT_WRIST_LEFT",
        "K4ABT_JOINT_HAND_LEFT",
        "K4ABT_JOINT_HANDTIP_LEFT",
        "K4ABT_JOINT_THUMB_LEFT",
        "K4ABT_JOINT_CLAVICLE_RIGHT",
        "K4ABT_JOINT_SHOULDER_RIGHT",
        "K4ABT_JOINT_ELBOW_RIGHT",
        "K4ABT_JOINT_WRIST_RIGHT",
        "K4ABT_JOINT_HAND_RIGHT",
        "K4ABT_JOINT_HANDTIP_RIGHT",
        "K4ABT_JOINT_THUMB_RIGHT",
        "K4ABT_JOINT_HIP_LEFT",
        "K4ABT_JOINT_KNEE_LEFT",
        "K4ABT_JOINT_ANKLE_LEFT",
        "K4ABT_JOINT_FOOT_LEFT",
        "K4ABT_JOINT_HIP_RIGHT",
        "K4ABT_JOINT_KNEE_RIGHT",
        "K4ABT_JOINT_ANKLE_RIGHT",
        "K4ABT_JOINT_FOOT_RIGHT",
        "K4ABT_JOINT_HEAD",
        "K4ABT_JOINT_NOSE",
        "K4ABT_JOINT_EYE_LEFT",
        "K4ABT_JOINT_EAR_LEFT",
        "K4ABT_JOINT_EYE_RIGHT",
        "K4ABT_JOINT_EAR_RIGHT"};
    
    std::unique_ptr<PoseJson> PoseData = std::make_unique<PoseJson>();

    for (int i = 0; i < K4ABT_JOINT_COUNT; ++i) {
        PoseData->joints[i].id = i;
        PoseData->joints[i].name = jointNames[i];

        
        PoseData->joints[i].position[0] = skeleton.joints[i].position.v[0];
        PoseData->joints[i].position[1] = skeleton.joints[i].position.v[1];
        PoseData->joints[i].position[2] = skeleton.joints[i].position.v[2];
    }

    crow::json::wvalue data = PoseData->toJson();
    WriteToFile(data);
}
