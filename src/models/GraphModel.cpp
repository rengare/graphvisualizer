#include "GraphModel.h"

const int GROUP_SIZE = 10;

GraphModel::GraphModel()
{
}

GraphModel::~GraphModel()
{
	glDeleteBuffers(1, &nodeSsbo);
}

GraphModel::GraphModel(Shader nodeShader, Shader edgeShader, ComputeShader computeShader, vector<VertexData> *data, vector<ConnectionIndices> *fromToConnections)
{
	this->nodeShader = nodeShader;
	this->edgeShader = edgeShader;
	this->computeShader = computeShader;

	this->bufferVertices = data;
	this->fromToConnections = fromToConnections;


	size = (*bufferVertices).size();
	fromToConnectionSize = (*fromToConnections).size();

	PrepareNodes();
}

void GraphModel::PrepareNodes() 
{
	glGenVertexArrays(1, &nodeVao);
	glBindVertexArray(nodeVao);

	glGenBuffers(1, &nodeSsbo);
	glGenBuffers(1, &fromToSsbo);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, nodeSsbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(VertexData), &(*bufferVertices)[0], GL_STATIC_DRAW);

	int positionLocation = glGetAttribLocation(nodeShader.GetShaderProgram(), "in_position");
	int colorLocation = glGetAttribLocation(nodeShader.GetShaderProgram(), "in_color");
	int sizeLocation = glGetAttribLocation(nodeShader.GetShaderProgram(), "in_size");

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(sizeLocation);

	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)0);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::color)));
	glVertexAttribPointer(sizeLocation, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::size)));

	glBindVertexArray(positionLocation);
	glBindVertexArray(colorLocation);
	glBindVertexArray(sizeLocation);
	
	glBindBuffer(GL_ARRAY_BUFFER, fromToSsbo);
	glBufferData(GL_ARRAY_BUFFER, fromToConnectionSize * sizeof(ConnectionIndices), &(*fromToConnections)[0], GL_STATIC_DRAW);


	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, nodeSsbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, fromToSsbo);

}

void GraphModel::PrepareEdges()
{

}

void GraphModel::Update() 
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, nodeSsbo);
	
	glUseProgram(computeShader.GetShaderProgram());

	glUniform1iv(glGetUniformLocation(computeShader.GetShaderProgram(), "graphDataSize"), 1, &size);
	glUniform1iv(glGetUniformLocation(computeShader.GetShaderProgram(), "connectionSize"), 1, &fromToConnectionSize);

	glDispatchCompute( (size / GROUP_SIZE) +1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	// VertexData *data = (VertexData *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size * sizeof(VertexData), bufMask);
}

void GraphModel::Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	DrawNodes(projection_matrix, view_matrix, cameraPosition);
	// DrawEdges(projection_matrix, view_matrix, cameraPosition);
}

void GraphModel::DrawNodes(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	glUseProgram(nodeShader.GetShaderProgram());

	glBindVertexArray(nodeVao);

	glUniformMatrix4fv(glGetUniformLocation(nodeShader.GetShaderProgram(), "view_matrix"), 1, false, &view_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(nodeShader.GetShaderProgram(), "projection_matrix"), 1, false, &projection_matrix[0][0]);

	glDrawArrays(GL_POINTS, 0, size);

	glBindVertexArray(0);
}

void GraphModel::DrawEdges(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	glUseProgram(edgeShader.GetShaderProgram());

	glBindVertexArray(nodeVao);

	glUniformMatrix4fv(glGetUniformLocation(edgeShader.GetShaderProgram(), "view_matrix"), 1, false, &view_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(edgeShader.GetShaderProgram(), "projection_matrix"), 1, false, &projection_matrix[0][0]);

	glDrawArrays(GL_LINES, 0, size);

	glBindVertexArray(0);
}

void GraphModel::Clear() 
{
	glDeleteBuffers(1, &nodeSsbo);
}