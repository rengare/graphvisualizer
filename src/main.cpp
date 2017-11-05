#include <vector>
#include <algorithm>
#include <json.hpp>

#include "helper/RandomNumberGenerator.h"
#include "config/AppConfig.h"
#include "graphic/GraphicsStructure.h"
#include "system/App.h"
#include "graphic/Shader.h"

using json = nlohmann::json;

int main(int argc, char *argv[])
{
    std::ifstream settingsFile("settings.json");
    json settingsJson;
    settingsFile >> settingsJson;

    AppConfig config;
    config.showGui = settingsJson["showGui"].get<bool>();
    config.width = settingsJson["width"].get<int>();
    config.height = settingsJson["height"].get<int>();
    config.positionX = settingsJson["positionX"].get<int>();
    config.positionY = settingsJson["positionY"].get<int>();
    config.isVsyncEnabled = settingsJson["isVsyncEnabled"].get<bool>();
    config.name = settingsJson["name"].get<std::string>();
    config.red = settingsJson["red"].get<float>();
    config.green = settingsJson["green"].get<float>();
    config.blue = settingsJson["blue"].get<float>();
    config.graphType3d = settingsJson["graphType3d"].get<bool>();
    config.showEdge = settingsJson["showEdge"].get<bool>();
    config.isUpdateOn = settingsJson["isUpdateOn"].get<bool>();
    config.nodeShaderName = settingsJson["nodeShaderName"].get<std::string>();
    config.lineShaderName = settingsJson["lineShaderName"].get<std::string>();
    config.nodeShaderVertexPath = settingsJson["nodeShaderVertexPath"].get<std::string>();
    config.nodeShaderFragmentPath = settingsJson["nodeShaderFragmentPath"].get<std::string>();
    config.lineShaderVertexPath = settingsJson["lineShaderVertexPath"].get<std::string>();
    config.lineShaderFragmentPath = settingsJson["lineShaderFragmentPath"].get<std::string>();
    config.edgeInput = settingsJson["edgeInput"].get<std::string>();
    config.nodeSizeRangeStart = settingsJson["nodeSizeRangeStart"].get<int>();
    config.nodeSizeRangeEnd = settingsJson["nodeSizeRangeEnd"].get<int>();

    std::vector<Connections> connections;
    std::vector<std::string> uniqueEdge;
    {
        std::ifstream inputFile(config.edgeInput);
        std::string x, y;
        while (inputFile >> x >> y)
        {
            Connections connect;
            connect.from = x;
            connect.to = y;

            uniqueEdge.push_back(x);
            uniqueEdge.push_back(y);

            connections.push_back(connect);
        }
    }

    std::sort(uniqueEdge.begin(), uniqueEdge.end());
    auto it = std::unique(uniqueEdge.begin(), uniqueEdge.end());
    uniqueEdge.resize((unsigned long)std::distance(uniqueEdge.begin(), it));

    map<string, int> edgeIndexMap;
    for (int i = 0; i < uniqueEdge.size(); i++)
    {
        edgeIndexMap[uniqueEdge[i]] = i;
    }

    std::vector<ConnectionIndices> fromToConnectionIndex;
    for (int i = 0; i < connections.size(); i++)
    {
        ConnectionIndices fromTo;
        fromTo.from = edgeIndexMap[connections[i].from];
        fromTo.to = edgeIndexMap[connections[i].to];
        fromToConnectionIndex.push_back(fromTo);
    }

    auto app = new App(config);
    Camera *camera = new Camera(config, 200000.0);
    camera->SetCameraVelocity(glm::vec3(100.f));
    camera->SetPosition(glm::vec3(0, 0, -700));
    app->AddCamera(camera);

    vector<VertexData> edgeData;
    vector<VertexData> nodeData(uniqueEdge.size());

    int hostArrayIndex = 0;

    for (int i = 0; i < uniqueEdge.size(); i++)
    {
        nodeData[i].vertexPosition.x = RandomNumberGenerator(-100, 100) / 1.0f;
        nodeData[i].vertexPosition.y = RandomNumberGenerator(-100, 100) / 1.0f;
        if (config.graphType3d)
        {
            nodeData[i].vertexPosition.z = RandomNumberGenerator(-100, 100) / 1.0f;
        }
        else
        {
            nodeData[i].vertexPosition.z = 0;
        }
        nodeData[i].color.r = RandomNumberGenerator(1, 10) / 10.f;
        nodeData[i].color.g = RandomNumberGenerator(1, 10) / 10.f;
        nodeData[i].color.b = RandomNumberGenerator(1, 10) / 10.f;
        nodeData[i].color.a = 1;
        nodeData[i].size = RandomNumberGenerator(config.nodeSizeRangeStart, config.nodeSizeRangeEnd);
        hostArrayIndex += 7;
    }

    for (int i = 0; i < fromToConnectionIndex.size(); i++)
    {
        edgeData.push_back(nodeData[fromToConnectionIndex[i].from]);
        edgeData.push_back(nodeData[fromToConnectionIndex[i].to]);
    }

    std::cout << edgeData.size() << endl;

    app->SetNodesCount((int)uniqueEdge.size());

    app->modelData = new ModelData(&nodeData, &edgeData, &fromToConnectionIndex);

    // app->graphModel = new FRModelCpu(config, &nodeData, &edgeData, &fromToConnectionIndex);

    app->SetAppState(AppState::RUN);
    app->Run();

    return 0;
}