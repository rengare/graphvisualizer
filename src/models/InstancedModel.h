#ifndef GRAPH_InstancedModelFromFile
#define GRAPH_InstancedModelFromFile

#include <iostream>
#include "../models/drawable/Model.h"
#include "../graphic/GraphicsStructure.h"
#include "../helper/RandomNumberGenerator.h"

class InstancedModel: public Model
{

public:
	InstancedModel();
	InstancedModel(int shaderProgram);
	InstancedModel(string fileName, string textureName, GLuint texture, int shaderProgram);
	~InstancedModel();

	void AddInstanced(Position position);
	void AddInstanced(Position position, Color color);
	void AddInstanced(Position position, Color color, int size);
	void AddInstanced(vector<VertexData>* data);
	void AddInstanced(VertexData data);

	vector<VertexData>* GetVertexData();

	void ShouldBindBuffers();
	void AddDataToBuffer();
	void BindInstancedBuffers();
	void UnbindInstancedBuffers();
	int GetInstancedSize();
	void SetDrawingMode(GLenum mode);
	
	virtual glm::vec3 GetPosition(int index) override;

	virtual void Draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::vec3& cameraPosition) override;
	virtual void Update() override;
	
	bool isVisible = true;

private:
	void Create(string fileName);

private:
	glm::vec3 rotation, rotation_speed, rotation_sin;

	vector<VertexData>* bufferVertices;

	bool bounded = false;
	GLenum drawingMode = GL_POINTS;
	GLuint vbo;
};
#endif // !GRAPH_InstancedModelFromFile
