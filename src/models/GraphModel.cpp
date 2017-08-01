#include "GraphModel.h"

GraphModel::GraphModel()
{
}

GraphModel::~GraphModel()
{
}

GraphModel::GraphModel(GLuint shader, GLuint compute, vector<VertexData> *data)
{
	shaderProgram = shader;
	computeShader = compute;
	bufferVertices = data;

	size = (*bufferVertices).size();

	glGenBuffers(1, &objectBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, objectBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(VertexData), &(*bufferVertices)[0], GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, objectBuffer);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, objectBuffer);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, objectBuffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)0);
	glBindVertexArray(0);
}

void GraphModel::Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	glBindVertexArray(vao);
	glUseProgram(computeShader);

	glDispatchCompute((size / 128) + 1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view_matrix"), 1, false, &view_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection_matrix"), 1, false, &projection_matrix[0][0]);

	glDrawArrays(drawingMode, 0, size);
	//glDrawArrays(GL_TRIANGLES, 0, verticesSize);

	glBindVertexArray(0);
}