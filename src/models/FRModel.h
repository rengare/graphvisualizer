#ifndef GRAPH_MODEL
#define GRAPH_MODEL

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

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
    FRModel(Shader nodeShader, Shader edgeShader, ComputeShader computeShader, vector<VertexData> *data, vector<ConnectionIndices> *fromToConnections);

	void PrepareNodes();

	void PrepareEdges();

    void Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition);

	void DrawNodes(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition);

	void DrawEdges(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition);
    
    void Update();

	void Clear();
	
    vector<VertexData> *GetVertexData();

    bool isVisible = true;

  private:
	vector<VertexData> *bufferVertices;
	vector<ConnectionIndices> *fromToConnections;

    GLenum drawingMode = GL_POINTS;
	
	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    GLuint nodeVao;
    GLuint nodeSsbo;
	GLuint fromToSsbo;

	Shader nodeShader, edgeShader;
	ComputeShader computeShader;

    int size;
	int fromToConnectionSize;
};
#endif
