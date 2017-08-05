#include "GraphModel.h"

GraphModel::GraphModel()
{
}

GraphModel::~GraphModel()
{
	glDeleteBuffers(1, &ssbo);
}

GraphModel::GraphModel(GLuint shader, GLuint compute, vector<VertexData> *data)
{
	shaderProgram = shader;
	computeShader = compute;
	bufferVertices = data;

	size = (*bufferVertices).size();

	glGenBuffers(1, &ssbo);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, ssbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(VertexData), &(*bufferVertices)[0], GL_STATIC_DRAW);
		

	int positionLocation = glGetAttribLocation(shaderProgram, "in_position");
	int colorLocation = glGetAttribLocation(shaderProgram, "in_color");
	int sizeLocation = glGetAttribLocation(shaderProgram, "in_size");

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(sizeLocation);

	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)0);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::color)));
	glVertexAttribPointer(sizeLocation, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::size)));

	glBindVertexArray(positionLocation);
	glBindVertexArray(colorLocation);
	glBindVertexArray(sizeLocation);

}

void GraphModel::Update() 
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	
	glUseProgram(computeShader);
	glUniform1iv(glGetUniformLocation(computeShader, "graphDataSize"), 1, &size);

	glDispatchCompute(size / 256, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	//VertexData *data = (VertexData *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size * sizeof(VertexData), bufMask);
}

void GraphModel::Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	glUseProgram(shaderProgram);

	glBindVertexArray(vao);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view_matrix"), 1, false, &view_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection_matrix"), 1, false, &projection_matrix[0][0]);

	glDrawArrays(drawingMode, 0, size);

	glBindVertexArray(0);
}

void GraphModel::Clear() 
{
	glDeleteBuffers(1, &ssbo);
}