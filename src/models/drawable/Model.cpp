#include "Model.h"

Model::Model()
{
}

Model::Model(string textureName, GLuint texture)
{
	SetTexture(textureName, texture);
}

Model::~Model()
{
	Destroy();
}

void Model::Update()
{
}

void Model::Draw()
{
}

void Model::Draw( const glm::mat4 & projectionMatrix, const glm::mat4 & viewMatrix, const glm::vec3& cameraPosition )
{
}

void Model::SetProgram( GLuint shaderProgram )
{
	this->shaderProgram = shaderProgram;
}

void Model::Destroy()
{
	glDeleteVertexArrays( 1, &vao );
	glDeleteBuffers( vbos.size(), &vbos[0] );
	vbos.clear();
}

GLuint Model::GetVao() const
{
	return vao;
}

const vector<GLuint>& Model::GetVbos() const
{
	return vbos;
}

void Model::SetTexture( string textureName, GLuint texture )
{
	textures[textureName] = texture;
}

const GLuint Model::GetTexture( string textureName ) const
{
	return textures.at( textureName );
}

void Model::SetModelName(string name)
{
	this->modelName = name;
}

string Model::GetModelName()
{
	return this->modelName;
}

