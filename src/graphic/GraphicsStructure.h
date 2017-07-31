#ifndef GRAPH_GRAPHICSSTRUCTURE_H
#define GRAPH_GRAPHICSSTRUCTURE_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
struct VertexData
{
	vec3 vertexPosition;
	vec4 color;
	float size;
	float dx = 0, dy = 0, dz = 0;

	VertexData(){};

	VertexData(vec3 pos)
	{
		vertexPosition = pos;
	}

	VertexData(vec3 pos, vec4 col)
	{
		vertexPosition = pos;
		color = col;
	}
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

#endif // !GRAPH_GRAPHICSSTRUCTURE_H
