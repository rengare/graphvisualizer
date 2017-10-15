#include "RandomModel.h"

const int GROUP_SIZE = 10;

RandomModel::RandomModel()
{
}

RandomModel::~RandomModel()
{
}

RandomModel::RandomModel(AppConfig config, vector<VertexData> *nodeData, vector<VertexData> *edgeData, vector<ConnectionIndices> *fromToConnections)
{
	this->nodeShader = new Shader(config.nodeShaderName, config.nodeShaderVertexPath, config.nodeShaderFragmentPath);
	this->edgeShader = new Shader(config.lineShaderName, config.lineShaderVertexPath, config.lineShaderFragmentPath);

	this->nodeCompute = new ComputeShader("res/shaders/random/random_positionupdate.comp");
	this->edgeCompute = new ComputeShader("res/shaders/random/random_lines.comp");

	this->bufferVertices = nodeData;
	this->edgeVertices = edgeData;
	this->fromToConnections = fromToConnections;
	this->config = config;

	nodeSize = (*bufferVertices).size();
	edgeSize = (*edgeVertices).size();
	fromToConnectionSize = (*fromToConnections).size();

	PrepareBuffers();

	PrepareEdges();
	PrepareNodes();
}

void RandomModel::PrepareBuffers()
{
	glGenBuffers(1, &nodeSsbo);
	glBindBuffer(GL_ARRAY_BUFFER, nodeSsbo);
	glBufferData(GL_ARRAY_BUFFER, nodeSize * sizeof(VertexData), &(*bufferVertices)[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &fromToSsbo);
	glBindBuffer(GL_ARRAY_BUFFER, fromToSsbo);
	glBufferData(GL_ARRAY_BUFFER, fromToConnectionSize * sizeof(ConnectionIndices), &(*fromToConnections)[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &edgeSsbo);
	glBindBuffer(GL_ARRAY_BUFFER, edgeSsbo);
	glBufferData(GL_ARRAY_BUFFER, edgeSize * sizeof(VertexData), &(*edgeVertices)[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &nodeVao);
	glGenVertexArrays(1, &edgeVao);
}

void RandomModel::PrepareNodes()
{
	glBindVertexArray(nodeVao);
	glBindBuffer(GL_ARRAY_BUFFER, nodeSsbo);

	int positionLocation = glGetAttribLocation(nodeShader->GetShaderProgram(), "in_position");
	int colorLocation = glGetAttribLocation(nodeShader->GetShaderProgram(), "in_color");
	int sizeLocation = glGetAttribLocation(nodeShader->GetShaderProgram(), "in_size");

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(sizeLocation);

	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)0);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::color)));
	glVertexAttribPointer(sizeLocation, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::size)));

	glBindVertexArray(positionLocation);
	glBindVertexArray(colorLocation);
	glBindVertexArray(sizeLocation);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, nodeSsbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, fromToSsbo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RandomModel::PrepareEdges()
{
	glBindVertexArray(edgeVao);
	glBindBuffer(GL_ARRAY_BUFFER, edgeSsbo);

	int positionLocation = glGetAttribLocation(edgeShader->GetShaderProgram(), "in_position");
	int colorLocation = glGetAttribLocation(edgeShader->GetShaderProgram(), "in_color");

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);

	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)0);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::color)));

	glBindVertexArray(positionLocation);
	glBindVertexArray(colorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, nodeSsbo);
	glBufferData(GL_ARRAY_BUFFER, nodeSize * sizeof(VertexData), &(*bufferVertices)[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, fromToSsbo);
	glBufferData(GL_ARRAY_BUFFER, fromToConnectionSize * sizeof(ConnectionIndices), &(*fromToConnections)[0], GL_STATIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, nodeSsbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, fromToSsbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, edgeSsbo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RandomModel::Update()
{
	if (config.isUpdateOn)
	{
		RandomPosition();
	}
}

void RandomModel::RandomPosition()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, nodeSsbo);
	glUseProgram(nodeCompute->GetShaderProgram());
	PassUniforms(nodeCompute->GetShaderProgram());
	glDispatchCompute((nodeSize / GROUP_SIZE) + 1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, edgeSsbo);
	glUseProgram(edgeCompute->GetShaderProgram());
	// PassUniforms(edgeCompute->GetShaderProgram());
	glDispatchCompute((fromToConnectionSize / GROUP_SIZE) + 1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void RandomModel::PassUniforms(GLuint shader)
{
	glUniform1iv(glGetUniformLocation(shader, "graphDataSize"), 1, &nodeSize);
	glUniform1iv(glGetUniformLocation(shader, "connectionSize"), 1, &fromToConnectionSize);

	glUniform1fv(11, 1, &limit);
}

void RandomModel::Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	DrawNodes(projection_matrix, view_matrix, cameraPosition);

	if (config.showEdge)
	{
		DrawEdges(projection_matrix, view_matrix, cameraPosition);
	};
}

void RandomModel::DrawNodes(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	glBindBuffer(GL_ARRAY_BUFFER, nodeSsbo);

	glUseProgram(nodeShader->GetShaderProgram());

	glBindVertexArray(nodeVao);

	glUniformMatrix4fv(glGetUniformLocation(nodeShader->GetShaderProgram(), "view_matrix"), 1, false, &view_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(nodeShader->GetShaderProgram(), "projection_matrix"), 1, false, &projection_matrix[0][0]);

	glDrawArrays(GL_POINTS, 0, nodeSize);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RandomModel::DrawEdges(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	glBindBuffer(GL_ARRAY_BUFFER, edgeSsbo);

	glUseProgram(edgeShader->GetShaderProgram());

	glBindVertexArray(edgeVao);

	glUniformMatrix4fv(glGetUniformLocation(edgeShader->GetShaderProgram(), "view_matrix"), 1, false, &view_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(edgeShader->GetShaderProgram(), "projection_matrix"), 1, false, &projection_matrix[0][0]);

	glDrawArrays(GL_LINES, 0, edgeSize);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RandomModel::DrawGui()
{
	ImGui::Begin("Graph settings");
	{
		ImGui::Checkbox("Show edge", &config.showEdge);
		ImGui::Checkbox("Update", &config.isUpdateOn);

		if (ImGui::InputFloat("Limit", &limit))
		{
			if (limit <= 0.0)
			{
				limit = 1;
			}
		};
	}

	ImGui::End();
}

void RandomModel::Clear()
{
	glDeleteBuffers(1, &nodeSsbo);
	glDeleteBuffers(1, &edgeSsbo);
	glDeleteBuffers(1, &fromToSsbo);

	glDeleteVertexArrays(1, &nodeVao);
	glDeleteVertexArrays(1, &edgeVao);

	nodeShader->Clear();
	edgeShader->Clear();

	nodeCompute->Clear();
	edgeCompute->Clear();
}