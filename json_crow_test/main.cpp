#include "crow.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

struct JointInfo
{
    char id;
    std::string name;
    std::vector<float>position = std::vector<float>(3);

    void reset(){    
        // std::fill(position.begin(), position.end(), 0.0f);
        position.assign(3, 0.0f);
    }

    crow::json::wvalue toJson()const{
        crow::json::wvalue node;
        node["id"] = id;
        node["name"] = name;
        node["position"] = position;

        return node;
    }

};

struct PoseJson{
    std::vector<JointInfo> joints = std::vector<JointInfo>(32);

    void rest(){
        for (auto&& node : joints){
            node.reset();
        }
    }

    crow::json::wvalue toJson() const{
        crow::json::wvalue data;

        std::vector<crow::json::wvalue> JointToJson;
        JointToJson.reserve(32);

        for ( auto&& node: joints){
            JointToJson.push_back(node.toJson());
        }


        data["joints"] = std::move(JointToJson);
        return data;
    }
};


void WriteToFile(const crow::json::wvalue& data){
    std::ofstream outFile("data.json");
    if (outFile.is_open()) {
        outFile << data.dump(2) << std::endl;
        outFile.close();
        std::cout << "Successfully wrote clean JSON using Crow!" << std::endl;
    } else {
        std::cerr << "Failed to open data.json for writing." << std::endl;
    }
}

int main() {
    // 1. Create a "write value" JSON object using Crow's built-in type
    
    // std::vector<crow::json::wvalue> joints;

    std::unique_ptr<PoseJson> PoseData = std::make_unique<PoseJson>();

    std::vector<std::string> jointNames = {
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
        "K4ABT_JOINT_EAR_RIGHT"
    };

    int index = 0;
    for( auto&& name : jointNames){
        PoseData.get()->joints[index].id = index;
        PoseData.get()->joints[index].name = name;

        // Example tracking data insertion
        PoseData->joints[index].position[0] = 1.23f * index; // X
        PoseData->joints[index].position[1] = 4.56f * index; // Y
        PoseData->joints[index].position[2] = 7.89f * index; // Z
        
        index++;
    }



    crow::json::wvalue data = PoseData->toJson();
    WriteToFile(data);

    // If you need to process another body frame, just call:
    // PoseData->reset();


    return 0;
}