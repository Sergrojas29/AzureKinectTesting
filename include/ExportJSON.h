#pragma once

#include "crow.h"
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <k4a/k4a.h>
#include <k4abt.h>

struct JointInfo
{
    int id;
    std::string name;
    std::array<float, 3> position;

    void reset()
    {
        position.fill(0.0f);
    }

    crow::json::wvalue toJson() const
    {
        crow::json::wvalue node;
        node["id"] = id;
        node["name"] = name;
        node["position"] = crow::json::wvalue::list({position[0], position[1], position[2]});;

        return node;
    }
};

struct PoseJson
{
    std::array<JointInfo, 32> joints;

    void reset()
    {
        for (auto &&node : joints)
        {
            node.reset();
        }
    }

    crow::json::wvalue toJson() const
    {
        crow::json::wvalue data;

        std::vector<crow::json::wvalue> JointToJson;
        JointToJson.reserve(32);

        for (auto &&node : joints)
        {
            JointToJson.push_back(node.toJson());
        }

        data["joints"] = std::move(JointToJson);
        return data;
    }
};

class ExportJSON
{
public:
    PoseJson PoseData;
    
    ExportJSON();

    void setOneFrame(const k4abt_skeleton_t& skeleton);
    
    void WriteToFile(std::string filename);

private:
    
};




