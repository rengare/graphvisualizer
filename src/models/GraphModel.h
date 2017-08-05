#ifndef GRAPH_MODEL
#define GRAPH_MODEL

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "../graphic/GraphicsStructure.h"
#include "../helper/RandomNumberGenerator.h"

using namespace std;
class GraphModel
{

  public:
    GraphModel();
    ~GraphModel();
    GraphModel(GLuint shader, GLuint compute, vector<VertexData> *data);

    void Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition);
    
    void Update();

	void Clear();
	
    vector<VertexData> *GetVertexData();

    bool isVisible = true;

  private:
    vector<VertexData> *bufferVertices;
    GLenum drawingMode = GL_POINTS;
	
	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    GLuint vao;
    GLuint ssbo;

    GLuint shaderProgram;
    GLuint computeShader;
    int size;
};
#endif
