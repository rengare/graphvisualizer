#ifndef FR_MODEL_CPU
#define FR_MODEL_CPU

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_sdl_gl3.h"

#include "../../config/AppConfig.h"
#include "../../graphic/GraphicsStructure.h"
#include "../../graphic/Shader.h"
#include "../../graphic/ComputeShader.h"
#include "../../helper/RandomNumberGenerator.h"

#include "../IModel.h"

using namespace std;

class FRModelCpu : public IModel
{
  public:
    FRModelCpu();
    ~FRModelCpu();
    FRModelCpu(AppConfig config, vector<VertexData> *nodeData, vector<VertexData> *edgeData, vector<ConnectionIndices> *fromToConnections);

    void Update() override;

    void Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition) override;

    void DrawGui() override;

    void Clear() override;

  private:
    void PrepareBuffers();

    void PrepareNodes();
    void PrepareEdges();
    
    void FruchtermanReingold();
    
    void UpdateNodes();
    void UpdateEdges();

    void DrawNodes(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition);
    void DrawEdges(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition);

    void PassUniforms(GLuint shader);

    vector<VertexData> *bufferVertices;
    vector<VertexData> *edgeVertices;
    vector<ConnectionIndices> *fromToConnections;

    GLuint nodeVao;
    GLuint nodeSbo;

    GLuint edgeVao;
    GLuint edgeSbo;

    Shader *nodeShader, *edgeShader;

    int nodeSize;
    int edgeSize;
    int fromToConnectionSize;
    AppConfig config;

    float SPEED_DIVISOR = 800;
    float AREA_MULTIPLICATOR = 10000;
    float speed = 1.0;
    float area = 100.0;
    float gravity = 10.0;  
};

#endif