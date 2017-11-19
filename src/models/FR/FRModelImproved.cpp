#include "FRModelImproved.h"

const int GROUP_SIZE = 256;

FRModelImproved::FRModelImproved()
{
}

FRModelImproved::~FRModelImproved()
{
}

FRModelImproved::FRModelImproved(AppConfig *config, vector<VertexData> *nodeData, vector<VertexData> *edgeData, vector<ConnectionIndices> *fromToConnections)
{
	this->nodeShader = new Shader(config->nodeShaderName, config->nodeShaderVertexPath, config->nodeShaderFragmentPath);
	this->edgeShader = new Shader(config->lineShaderName, config->lineShaderVertexPath, config->lineShaderFragmentPath);

	this->repulsiveCompute = new ComputeShader("res/shaders/fruchterman-reingold/improved/fruchtermanreingold_distance_calc.comp");
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

	pc.pts = &(*bufferVertices)[0];
	pc.size = nodeSize;
	pc2kd = new PC2KD(pc); // The adaptor
	index = new my_kd_tree_t(3 /*dim*/, *pc2kd, KDTreeSingleIndexAdaptorParams(10 /* max leaf */));

	PrepareBuffers();

	PrepareEdges();
	PrepareNodes();
}

void FRModelImproved::PrepareBuffers()
{
	glGenBuffers(1, &nodeSsbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, nodeSsbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, nodeSize * sizeof(VertexData), &pc.pts[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &fromToSsbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fromToSsbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, fromToConnectionSize * sizeof(ConnectionIndices), &(*fromToConnections)[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &edgeSsbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, edgeSsbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, edgeSize * sizeof(VertexData), &(*edgeVertices)[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenVertexArrays(1, &nodeVao);
	glGenVertexArrays(1, &edgeVao);
}

void FRModelImproved::PrepareNodes()
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
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 15, repulsiveSsbo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void FRModelImproved::PrepareEdges()
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

void FRModelImproved::Update()
{
	graphType = config->graphType3d;

	if (config->isUpdateOn)
	{
		UpdateNodes();
		UpdateEdges();
	};
}

void FRModelImproved::UpdateNodes()
{

	index->buildIndex();

	// float k = (float)std::sqrt((AREA_MULTIPLICATOR * area) / (1.f + nodeSize));
	float maxDisplace = (float)(std::sqrt(500 * area) / 10.f); // Déplacement limite : on peut le calibrer...

	float k = (float)std::sqrt((500 * area) / (1.f + nodeSize));

	for (int i = 0; i < nodeSize; i++)
	{
		pc.pts[i].dx = 0;
		pc.pts[i].dy = 0;
		pc.pts[i].dz = 0;

		if (config->graphType3d && pc.pts[i].vertexPosition.z == 0.0)
		{
			pc.pts[i].vertexPosition.z = 1;
		}

		float query_pt[3] = {pc.pts[i].vertexPosition.x, pc.pts[i].vertexPosition.y, pc.pts[i].vertexPosition.z};

		{
			size_t num_results = 25;
			std::vector<size_t> ret_index(num_results);
			std::vector<float> out_dist_sqr(num_results);

			num_results = index->knnSearch(&query_pt[0], num_results, &ret_index[0], &out_dist_sqr[0]);

			// In case of less points in the tree than requested:
			ret_index.resize(num_results);
			// out_dist_sqr.resize(num_results);
			for (int j = 0; j < num_results; j++)
			{

				float xDist = pc.pts[i].vertexPosition.x - pc.pts[ret_index[j]].vertexPosition.x;
				float yDist = pc.pts[i].vertexPosition.y - pc.pts[ret_index[j]].vertexPosition.y;
				float zDist = pc.pts[i].vertexPosition.z - pc.pts[ret_index[j]].vertexPosition.z;
				float dist = 0;

				if (config->graphType3d)
				{
					dist = std::sqrt(xDist * xDist + yDist * yDist + zDist * zDist);
				}
				else
				{
					dist = std::sqrt(xDist * xDist + yDist * yDist);
				}

				if (dist > 0)
				{
					float repulsive = (k * k) / dist;
					pc.pts[i].dx += xDist / dist * repulsive;
					pc.pts[i].dy += yDist / dist * repulsive;
					if (config->graphType3d)
						pc.pts[i].dz += zDist / dist * repulsive;
				}
			}
		}
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, nodeSsbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, nodeSize * sizeof(VertexData), &pc.pts[0], GL_DYNAMIC_DRAW);

	// // repulsive
	// glUseProgram(repulsiveCompute->GetShaderProgram());
	// glDispatchCompute((nodeSize / GROUP_SIZE) + 1, 1, 1);
	// PassUniforms(repulsiveCompute->GetShaderProgram());
	// glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

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

	GLint bufMask = GL_MAP_WRITE_BIT;
	// the invalidate makes a big difference when re-writing
	pc.pts = (VertexData *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, nodeSize * sizeof(VertexData), bufMask);
	int i = 1;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void FRModelImproved::UpdateEdges()
{

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, edgeSsbo);

	//lines
	glUseProgram(linesCompute->GetShaderProgram());
	PassUniforms(linesCompute->GetShaderProgram());
	glDispatchCompute((fromToConnectionSize / GROUP_SIZE) + 1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void FRModelImproved::PassUniforms(GLuint shader)
{
	glUniform1iv(glGetUniformLocation(shader, "graphDataSize"), 1, &nodeSize);
	glUniform1iv(glGetUniformLocation(shader, "connectionSize"), 1, &fromToConnectionSize);

	glUniform1fv(11, 1, &speed);
	glUniform1fv(12, 1, &area);
	glUniform1fv(13, 1, &gravity);
	glUniform1iv(17, 1, &graphType);
}

void FRModelImproved::Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	DrawNodes(projection_matrix, view_matrix, cameraPosition);

	if (config->showEdge)
	{
		DrawEdges(projection_matrix, view_matrix, cameraPosition);
	};
}

void FRModelImproved::DrawNodes(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, nodeSsbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, nodeSize * sizeof(VertexData), &pc.pts[0], GL_DYNAMIC_DRAW);

	glUseProgram(nodeShader->GetShaderProgram());

	glBindVertexArray(nodeVao);

	glUniformMatrix4fv(glGetUniformLocation(nodeShader->GetShaderProgram(), "view_matrix"), 1, false, &view_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(nodeShader->GetShaderProgram(), "projection_matrix"), 1, false, &projection_matrix[0][0]);

	glDrawArrays(GL_POINTS, 0, nodeSize);

	glBindVertexArray(0);
	glUseProgram(0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void FRModelImproved::DrawEdges(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
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

void FRModelImproved::DrawGui()
{
	ImGui::Begin("Graph settings");
	{
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
			if (area <= 0.0)
			{
				area = 0.1;
			}
		};
		ImGui::InputFloat("gravity", &gravity);
	}

	ImGui::End();
}

void FRModelImproved::Clear()
{
	glDeleteBuffers(1, &nodeSsbo);
	glDeleteBuffers(1, &edgeSsbo);
	glDeleteBuffers(1, &fromToSsbo);
	glDeleteBuffers(1, &repulsiveSsbo);

	glDeleteVertexArrays(1, &nodeVao);
	glDeleteVertexArrays(1, &edgeVao);

	nodeShader->Clear();
	edgeShader->Clear();

	repulsiveCompute->Clear();
	attractiveCompute->Clear();
	updateCompute->Clear();
	linesCompute->Clear();
}