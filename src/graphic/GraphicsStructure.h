#ifndef GRAPH_GRAPHICSSTRUCTURE_H
#define GRAPH_GRAPHICSSTRUCTURE_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Position
{
	glm::vec3 position;

	Position()
	{
	}

	Position(const glm::vec3 &pos)
	{
		position = pos;
	}

	void SetPosition(float x, float y, float z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}
};

struct Color
{
	glm::vec4 color;

	Color()
	{
	}

	Color(const glm::vec4 &col)
	{
		color = col;
	}

	void SetColor(float r, float g, float b, float aplha)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = aplha;
	}
};

struct VertexData
{
	Position vertexPosition;
	Color color;
	float dx = 0, dy = 0, dz = 0;
	float size;

	VertexData(){};

	VertexData(Position pos)
	{
		vertexPosition = pos;
	}

	VertexData(Position pos, Color col)
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
