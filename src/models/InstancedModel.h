#ifndef GRAPH_InstancedModelFromFile
#define GRAPH_InstancedModelFromFile

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "../graphic/GraphicsStructure.h"
#include "../helper/RandomNumberGenerator.h"

using namespace std;
class InstancedModel
{

  public:
	InstancedModel();
	InstancedModel(int shaderProgram);
	~InstancedModel();

	void AddInstanced(vector<VertexData> *data);
	void AddInstanced(VertexData data);

	vector<VertexData> *GetVertexData();

	void ShouldBindBuffers();
	void AddDataToBuffer();
	void BindInstancedBuffers();
	void UnbindInstancedBuffers();
	int GetInstancedSize();
	void SetDrawingMode(GLenum mode);
	void AddComputeShader(GLuint shader);

	glm::vec3 GetPosition(int index);

	void Draw(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix, const glm::vec3 &cameraPosition);
	void Update();

	bool isVisible = true;

  private:
	void Create(string fileName);

  private:
	glm::vec3 rotation_speed, rotation_sin;

	vector<VertexData> *bufferVertices;

	bool bounded = false;
	GLenum drawingMode = GL_POINTS;
	GLuint vbo;

	GLuint vao;
	vector<GLuint> vbos;
	GLuint shaderProgram;
	GLuint computeShader;

	string textureName;
	string modelPath;
	string modelName;
	string modelFileName;
	map<string, GLuint> textures;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec4 color;

	GLuint structDataSSBO;
};
#endif // !GRAPH_InstancedModelFromFile
