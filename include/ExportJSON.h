#pragma once

#include "crow.h"
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <k4a/k4a.h>
#include <k4abt.h>

struct JointInfo
{
    char id;
    std::string name;
    std::vector<float> position = std::vector<float>(3);

    void reset()
    {
        position.assign(3, 0.0f);
    }

    crow::json::wvalue toJson() const
    {
        crow::json::wvalue node;
        node["id"] = id;
        node["name"] = name;
        node["position"] = position;

        return node;
    }
};

struct PoseJson
{
    std::vector<JointInfo> joints = std::vector<JointInfo>(32);

    void rest()
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
    ExportJSON() = delete;

    static void getOneFrame(const k4abt_skeleton_t& skeleton);
    
    static void WriteToFile(const crow::json::wvalue &data);

private:
    
};