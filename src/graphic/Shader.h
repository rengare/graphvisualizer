#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../helper/FileHelper.h"
#include "../helper/Logger.h"

class Shader
{
public:
	Shader();
	Shader(string shaderProgramName, string vertexfileName, string fragmentfileName);
	~Shader();

	GLint GetAttributeLocationBy(string name);
	GLint GetUniformLocationBy(string name);

	void SetUniformMatrix(GLint location, glm::mat4 matrix);
	void SetUniformMatrix(string name, glm::mat4 matrix);
	void SetAttributeBy(GLint location, string name);
	
	GLuint GetShaderProgram();

private:
	bool LoadVertexShader(GLuint& shaderProgram, string fileName);
	bool LoadFragmentShader(GLuint& shaderProgram, string fileName);

	bool CompileVertexShader(GLuint& shaderProgram, string& fileContent);
	bool CompileFragmentShader(GLuint& shaderProgram, string& fileContent);
	bool Compile(GLuint& shaderProgram, GLuint& loadedShader, const char* source);

	void AttachCompiledShader(GLuint& shaderProgram, GLuint& compiledShader);

	bool Link(GLuint& shaderProgram);

private:
	GLuint shaderProgram;
};

#endif