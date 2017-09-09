#ifndef GRAPH_MODEL
#define GRAPH_MODEL

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl_gl3.h"

#include "../config/AppConfig.h"
#include "../graphic/GraphicsStructure.h"
#include "../graphic/Shader.h"
#include "../graphic/ComputeShader.h"
#include "../helper/RandomNumberGenerator.h"

using namespace std;
class FRModel
{

  public:
    FRModel();
    ~FRModel();
    FRModel(AppConfig config, vector<VertexData> *nodeData, vector<VertexData> *edgeData, vector<ConnectionIndices> *fromToConnections);

    void PrepareNodes();

    void PrepareEdges();

    void Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition);

    void DrawGui();

    void Update();

    void UpdateNodes();
    void UpdateEdges();

    void Clear();

    vector<VertexData> *GetVertexData();

    bool isVisible = true;

  private:
    void DrawNodes(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition);

    void DrawEdges(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition);

    void PassUniforms(GLuint shader);

    vector<VertexData> *bufferVertices;
    vector<VertexData> *edgeVertices;
    vector<ConnectionIndices> *fromToConnections;

    GLenum drawingMode = GL_POINTS;

    GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    GLuint nodeVao;
    GLuint nodeSsbo;
    GLuint fromToSsbo;

    GLuint edgeVao;
    GLuint edgeSsbo;

    Shader *nodeShader, *edgeShader;

    ComputeShader *repulsiveCompute;
    ComputeShader *attractiveCompute;
    ComputeShader *updateCompute;
    ComputeShader *linesCompute;

    int nodeSize;
    int edgeSize;
    int fromToConnectionSize;
    AppConfig config;

    float speed = 1.0;
    float area = 100.0;
    float gravity = 10.0;
};
#endif
