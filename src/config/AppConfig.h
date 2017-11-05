#ifndef WINDOWSTRUCTURES_H
#define WINDOWSTRUCTURES_H

#include <string>

struct AppConfig
{
    int width = 1280;
    int height = 620;
    int positionX = 400;
    int positionY = 100;
    bool showGui = true;
    bool isVsyncEnabled = true;
    std::string name = "The project";
    float red = 0.f;
    float green = 0.f;
    float blue = 0.f;
    float skip;
    bool graphType3d = false;
    bool showEdge = true;
    bool isUpdateOn = true;
    int nodeSizeRangeStart = 1;
    int nodeSizeRangeEnd = 1;
    std::string nodeShaderName = "";
    std::string lineShaderName = "";
    std::string nodeShaderVertexPath = "";
    std::string nodeShaderFragmentPath = "";
    std::string lineShaderVertexPath = "";
    std::string lineShaderFragmentPath = "";
    std::string edgeInput;
};

#endif