#include "FRModel.h"

const int GROUP_SIZE = 10;

FRModel::FRModel()
{
}

FRModel::~FRModel()
{
}

FRModel::FRModel(AppConfig config, vector<VertexData> *nodeData, vector<VertexData> *edgeData, vector<ConnectionIndices> *fromToConnections)
{
	this->nodeShader = new Shader(config.nodeShaderName, config.nodeShaderVertexPath, config.nodeShaderFragmentPath);
	this->edgeShader = new Shader(config.lineShaderName, config.lineShaderVertexPath, config.lineShaderFragmentPath);

	this->repulsiveCompute = new ComputeShader("res/shaders/fruchterman-reingold/fruchtermanreingold_repulsive.comp");
	this->attractiveCompute = new ComputeShader("res/shaders/fruchterman-reingold/fruchtermanreingold_attractive.comp");
	this->updateCompute = new ComputeShader("res/shaders/fruchterman-reingold/fruchtermanreingold_positionupdate.comp");
	this->linesCompute = new ComputeShader("res/shaders/fruchterman-reingold/fruchtermanreingold_lines.comp");

	this->bufferVertices = nodeData;
	this->edgeVertices = edgeData;
	this->fromToConnections = fromToConnections;
	this->config = config;

	nodeSize = (*bufferVertices).size();
	edgeSize = (*edgeVertices).size();
	fromToConnectionSize = (*fromToConnections).size();

	PrepareEdges();
	PrepareNodes();
}

void FRModel::PrepareNodes()
{
	glGenVertexArrays(1, &nodeVao);
	glBindVertexArray(nodeVao);

	glGenBuffers(1, &nodeSsbo);
	glGenBuffers(1, &fromToSsbo);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, nodeSsbo);
	glBufferData(GL_ARRAY_BUFFER, nodeSize * sizeof(VertexData), &(*bufferVertices)[0], GL_STATIC_DRAW);

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

	glBindBuffer(GL_ARRAY_BUFFER, fromToSsbo);
	glBufferData(GL_ARRAY_BUFFER, fromToConnectionSize * sizeof(ConnectionIndices), &(*fromToConnections)[0], GL_STATIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, nodeSsbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, fromToSsbo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void FRModel::PrepareEdges()
{
	glGenVertexArrays(1, &edgeVao);
	glBindVertexArray(edgeVao);

	glGenBuffers(1, &edgeSsbo);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, edgeSsbo);
	glBufferData(GL_ARRAY_BUFFER, edgeSize * sizeof(VertexData), &(*edgeVertices)[0], GL_STATIC_DRAW);

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
}

void FRModel::Update()
{
	if (config.isUpdateOn)
	{
		UpdateNodes();
		UpdateEdges();
	};
}

void FRModel::UpdateNodes()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, nodeSsbo);

	//repulsive
	glUseProgram(repulsiveCompute->GetShaderProgram());
	PassUniforms(repulsiveCompute->GetShaderProgram());
	glDispatchCompute((nodeSize / GROUP_SIZE) + 1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//attractive
	glUseProgram(attractiveCompute->GetShaderProgram());
	PassUniforms(attractiveCompute->GetShaderProgram());
	glDispatchCompute((fromToConnectionSize / GROUP_SIZE) + 1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//update
	glUseProgram(updateCompute->GetShaderProgram());
	PassUniforms(updateCompute->GetShaderProgram());
	glDispatchCompute((nodeSize / GROUP_SIZE) + 1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void FRModel::UpdateEdges()
{

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, edgeSsbo);

	//lines
	glUseProgram(linesCompute->GetShaderProgram());
	PassUniforms(linesCompute->GetShaderProgram());
	glDispatchCompute((fromToConnectionSize / GROUP_SIZE) + 1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void FRModel::PassUniforms(GLuint shader)
{
	glUniform1iv(glGetUniformLocation(shader, "graphDataSize"), 1, &nodeSize);
	glUniform1iv(glGetUniformLocation(shader, "connectionSize"), 1, &fromToConnectionSize);

	glUniform1fv(11, 1, &speed);
	glUniform1fv(12, 1, &area);
	glUniform1fv(13, 1, &gravity);
}

void FRModel::Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	DrawNodes(projection_matrix, view_matrix, cameraPosition);

	if (config.showEdge)
	{
		DrawEdges(projection_matrix, view_matrix, cameraPosition);
	};
}

void FRModel::DrawNodes(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	glBindBuffer(GL_ARRAY_BUFFER, nodeSsbo);

	glUseProgram(nodeShader->GetShaderProgram());

	glBindVertexArray(nodeVao);

	glUniformMatrix4fv(glGetUniformLocation(nodeShader->GetShaderProgram(), "view_matrix"), 1, false, &view_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(nodeShader->GetShaderProgram(), "projection_matrix"), 1, false, &projection_matrix[0][0]);

	glDrawArrays(GL_POINTS, 0, nodeSize);

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FRModel::DrawEdges(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	glBindBuffer(GL_ARRAY_BUFFER, edgeSsbo);

	glUseProgram(edgeShader->GetShaderProgram());

	glBindVertexArray(edgeVao);

	glUniformMatrix4fv(glGetUniformLocation(edgeShader->GetShaderProgram(), "view_matrix"), 1, false, &view_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(edgeShader->GetShaderProgram(), "projection_matrix"), 1, false, &projection_matrix[0][0]);

	glDrawArrays(GL_LINES, 0, edgeSize);

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FRModel::DrawGui()
{
	ImGui::Begin("Graph settings");
	{
		ImGui::Checkbox("Show edge", &config.showEdge);
		ImGui::Checkbox("Update", &config.isUpdateOn);

		if (ImGui::InputFloat("speed", &speed))
		{
			if (speed < 0.0)
			{
				speed = 0.1;
			}
			if (speed > 1000.0)
			{
				speed = 1000.0;
			}
		};

		if (ImGui::InputFloat("area", &area))
		{
			if(area <= 0.0){
				area = 0.1;
			}
		};
		ImGui::InputFloat("gravity", &gravity);
	}

	ImGui::End();
}

void FRModel::Clear()
{
	glDeleteBuffers(1, &nodeSsbo);
	glDeleteBuffers(1, &edgeSsbo);
	glDeleteBuffers(1, &fromToSsbo);

	glDeleteBuffers(1, &nodeVao);
	glDeleteBuffers(1, &edgeVao);

	nodeShader->Clear();
	edgeShader->Clear();

	repulsiveCompute->Clear();
	attractiveCompute->Clear();
	updateCompute->Clear();
	linesCompute->Clear();
	
}