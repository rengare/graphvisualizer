#ifndef MODEL_H
#define MODEL_H

#include <map>

#include "IDrawableObject.h"

using namespace std;

class Model : public IDrawableObject
{
public:
	Model();
	Model(string textureName, GLuint texture );
	virtual ~Model();
	virtual void Draw() override;
	virtual void Draw( const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::vec3& cameraPosition ) override;
	virtual void SetProgram( GLuint shaderProgram ) override;

	virtual void Update() override;
	virtual void Destroy() override;

	virtual GLuint GetVao() const override;
	virtual const vector<GLuint>& GetVbos() const override;

    virtual glm::vec3 GetPosition(int index) = 0;

	virtual void SetTexture( string textureName, GLuint texture ) override;
	virtual const GLuint GetTexture( string textureName ) const override;

	virtual void SetModelName(string name);
	virtual string GetModelName();
	
	float mass;

protected:
	GLuint vao;
	vector<GLuint> vbos;
	GLuint shaderProgram;

	string textureName;
	string modelPath;
	string modelName;
	string modelFileName;
	map<string, GLuint> textures;
	
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec4 color;
};

#endif // !MODEL_H
