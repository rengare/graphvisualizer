#ifndef GRAPH_GRAPHICSSTRUCTURE_H
#define GRAPH_GRAPHICSSTRUCTURE_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace glm;
struct VertexData
{
	vec4 vertexPosition = vec4(0, 0, 0, 0);
	vec4 color = vec4(0, 0, 0, 1);
	float size = 0;
	float dx = 0, dy = 0, dz = 0;

	VertexData(){};
};

struct Connections
{
	std::string from;
	std::string to;
};

struct ConnectionIndices
{
	int from;
	int to;
};

struct ModelData
{
	std::vector<VertexData> *nodeData;
	std::vector<VertexData> *edgeData;
	std::vector<ConnectionIndices> *fromToConnections;

	ModelData()
	{
	}

	ModelData(
		std::vector<VertexData> *nodeData,
		std::vector<VertexData> *edgeData,
		std::vector<ConnectionIndices> *fromToConnections)
	{
		this->nodeData = nodeData;
		this->edgeData = edgeData;
		this->fromToConnections = fromToConnections;
	}
};

#endif // !GRAPH_GRAPHICSSTRUCTURE_H
