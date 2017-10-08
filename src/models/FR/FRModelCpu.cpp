#include "FRModelCpu.h"

const int GROUP_SIZE = 10;

FRModelCpu::FRModelCpu()
{
}

FRModelCpu::~FRModelCpu()
{
	glDeleteBuffers(1, &nodeSbo);
}

FRModelCpu::FRModelCpu(AppConfig config, vector<VertexData> *nodeData, vector<VertexData> *edgeData, vector<ConnectionIndices> *fromToConnections)
{
	this->nodeShader = new Shader(config.nodeShaderName, config.nodeShaderVertexPath, config.nodeShaderFragmentPath);
	this->edgeShader = new Shader(config.lineShaderName, config.lineShaderVertexPath, config.lineShaderFragmentPath);

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

void FRModelCpu::PrepareBuffers()
{
	glGenBuffers(1, &nodeSbo);
	glBindBuffer(GL_ARRAY_BUFFER, nodeSbo);
	glBufferData(GL_ARRAY_BUFFER, nodeSize * sizeof(VertexData), &(*bufferVertices)[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &edgeSbo);
	glBindBuffer(GL_ARRAY_BUFFER, edgeSbo);
	glBufferData(GL_ARRAY_BUFFER, edgeSize * sizeof(VertexData), &(*edgeVertices)[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &nodeVao);
	glGenVertexArrays(1, &edgeVao);	
}

void FRModelCpu::PrepareNodes()
{
	glBindVertexArray(nodeVao);
	glBindBuffer(GL_ARRAY_BUFFER, nodeSbo);
	
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glBindVertexArray(0);
}

void FRModelCpu::PrepareEdges()
{
	glBindVertexArray(edgeVao);
	glBindBuffer(GL_ARRAY_BUFFER, edgeSbo);

	int positionLocation = glGetAttribLocation(edgeShader->GetShaderProgram(), "in_position");
	int colorLocation = glGetAttribLocation(edgeShader->GetShaderProgram(), "in_color");

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);

	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)0);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(offsetof(VertexData, VertexData::color)));

	glBindVertexArray(positionLocation);
	glBindVertexArray(colorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glBindVertexArray(0);
}

void FRModelCpu::Update()
{
	if (config.isUpdateOn)
	{
		FruchtermanReingold();
	};
}

void FRModelCpu::PassUniforms(GLuint shader)
{
	glUniform1iv(glGetUniformLocation(shader, "graphDataSize"), 1, &nodeSize);
	glUniform1iv(glGetUniformLocation(shader, "connectionSize"), 1, &fromToConnectionSize);

	glUniform1fv(11, 1, &speed);
	glUniform1fv(12, 1, &area);
	glUniform1fv(13, 1, &gravity);
}

void FRModelCpu::Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	DrawNodes(projection_matrix, view_matrix, cameraPosition);

	if (config.showEdge)
	{
		DrawEdges(projection_matrix, view_matrix, cameraPosition);
	};
}

void FRModelCpu::DrawNodes(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	glBindBuffer(GL_ARRAY_BUFFER, nodeSbo);

	glUseProgram(nodeShader->GetShaderProgram());

	glBindVertexArray(nodeVao);

	glUniformMatrix4fv(glGetUniformLocation(nodeShader->GetShaderProgram(), "view_matrix"), 1, false, &view_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(nodeShader->GetShaderProgram(), "projection_matrix"), 1, false, &projection_matrix[0][0]);

	glDrawArrays(GL_POINTS, 0, nodeSize);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FRModelCpu::DrawEdges(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix, const glm::vec3 &cameraPosition)
{
	glBindBuffer(GL_ARRAY_BUFFER, edgeSbo);

	glUseProgram(edgeShader->GetShaderProgram());

	glBindVertexArray(edgeVao);

	glUniformMatrix4fv(glGetUniformLocation(edgeShader->GetShaderProgram(), "view_matrix"), 1, false, &view_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(edgeShader->GetShaderProgram(), "projection_matrix"), 1, false, &projection_matrix[0][0]);

	glDrawArrays(GL_LINES, 0, edgeSize);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FRModelCpu::DrawGui()
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
			if (area <= 0.0)
			{
				area = 0.1;
			}
		};
		ImGui::InputFloat("gravity", &gravity);
	}

	ImGui::End();
}

void FRModelCpu::Clear()
{
	glDeleteBuffers(1, &nodeSbo);
	glDeleteBuffers(1, &edgeSbo);

	glDeleteVertexArrays(1, &nodeVao);
	glDeleteVertexArrays(1, &edgeVao);

	nodeShader->Clear();
	edgeShader->Clear();
}

void FRModelCpu::FruchtermanReingold()
{
	auto nodeSize = bufferVertices->size();

	(*edgeVertices).clear();

	float maxDisplace = (float)(std::sqrt(AREA_MULTIPLICATOR * area) / 10.f); // Déplacement limite : on peut le calibrer...
	float k = (float)std::sqrt((AREA_MULTIPLICATOR * area) / (1.f + nodeSize));

	for (int i = 0; i < nodeSize; i++)
	{
		(*bufferVertices)[i].dx = 0;
		(*bufferVertices)[i].dy = 0;
		(*bufferVertices)[i].dz = 0;

		for (int j = 0; j < nodeSize; j++)
		{
			if (i != j)
			{
				float xDist = (*bufferVertices)[i].vertexPosition.x - (*bufferVertices)[j].vertexPosition.x;
				float yDist = (*bufferVertices)[i].vertexPosition.y - (*bufferVertices)[j].vertexPosition.y;
				float zDist = (*bufferVertices)[i].vertexPosition.z - (*bufferVertices)[j].vertexPosition.z;
				float dist = std::sqrt(xDist * xDist + yDist * yDist + zDist * zDist);
				if (dist > 0)
				{
					float repulsive = (k * k) / dist;
					(*bufferVertices)[i].dx += xDist / dist * repulsive;
					(*bufferVertices)[i].dy += yDist / dist * repulsive;
					(*bufferVertices)[i].dz += zDist / dist * repulsive;
				}
			}
		}
	}

	auto edgeSize = (*fromToConnections).size();
	for (int i = 0; i < edgeSize; i++)
	{
		auto source = (*bufferVertices)[(*fromToConnections)[i].from];
		auto target = (*bufferVertices)[(*fromToConnections)[i].to];

		float xDist = source.vertexPosition.x - target.vertexPosition.x;
		float yDist = source.vertexPosition.y - target.vertexPosition.y;
		float zDist = source.vertexPosition.z - target.vertexPosition.z;
		float dist = std::sqrt(xDist * xDist + yDist * yDist + zDist * zDist);

		if (dist > 0)
		{
			float atractive = (dist * dist) / k;
			source.dx -= xDist / dist * atractive;
			source.dy -= yDist / dist * atractive;
			source.dz -= zDist / dist * atractive;

			target.dx += xDist / dist * atractive;
			target.dy += yDist / dist * atractive;
			target.dz += zDist / dist * atractive;

			(*bufferVertices)[(*fromToConnections)[i].from] = source;
			(*bufferVertices)[(*fromToConnections)[i].to] = target;
		}
	}

	for (int i = 0; i < nodeSize; i++)
	{
		auto pos = (*bufferVertices)[i];
		float d = std::sqrt((pos.vertexPosition.x * pos.vertexPosition.x + pos.vertexPosition.y * pos.vertexPosition.y + pos.vertexPosition.z * pos.vertexPosition.z));
		float gf = 0.01f * k * (float)gravity * d;

		pos.dx -= gf * pos.vertexPosition.x / d;
		pos.dy -= gf * pos.vertexPosition.y / d;
		pos.dz -= gf * pos.vertexPosition.z / d;

		pos.dx += speed;
		pos.dy += speed;
		pos.dz += speed;

		d = std::sqrt((pos.dx * pos.dx + pos.dy * pos.dy + pos.dz * pos.dz));

		if (d > 0)
		{

#ifdef _WIN32
			float limitedDist = min(maxDisplace * ((float)speed / SPEED_DIVISOR), d);
#else
			float limitedDist = std::min(maxDisplace * ((float)speed / SPEED_DIVISOR), d);
#endif

			pos.vertexPosition.x = pos.vertexPosition.x + pos.dx / d * limitedDist;
			pos.vertexPosition.y = pos.vertexPosition.y + pos.dy / d * limitedDist;
			if (config.graphType3d)
			{
				pos.vertexPosition.z = pos.vertexPosition.z + pos.dz / d * limitedDist;
			}
		}
		(*bufferVertices)[i] = pos;
	}

	for (int i = 0; i < (*fromToConnections).size(); i++)
	{
		auto source = (*bufferVertices)[(*fromToConnections)[i].from];
		auto target = (*bufferVertices)[(*fromToConnections)[i].to];

		(*edgeVertices).push_back(source);
		(*edgeVertices).push_back(target);
	}

	UpdateNodes();
	UpdateEdges();
}

void FRModelCpu::UpdateNodes()
{
	glBindBuffer(GL_ARRAY_BUFFER, nodeSbo);
	glBufferData(GL_ARRAY_BUFFER, nodeSize * sizeof(VertexData), &(*bufferVertices)[0], GL_STATIC_DRAW);
}

void FRModelCpu::UpdateEdges()
{
	glBindBuffer(GL_ARRAY_BUFFER, edgeSbo);
	glBufferData(GL_ARRAY_BUFFER, edgeSize * sizeof(VertexData), &(*edgeVertices)[0], GL_STATIC_DRAW);
}
