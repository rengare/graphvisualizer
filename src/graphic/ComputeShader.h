#ifndef COMPUTE_SHADER_H
#define COMPUTE_SHADER_H

#include <string>
#include <vector>
#include <map>

#include <glad/glad.h>

#include "../helper/FileHelper.h"
#include "../helper/Logger.h"

class ComputeShader
{
public:
    ComputeShader(string shaderFile);
private:
	bool LoadShader(string fileName);

	bool CompileShader(string& fileContent);
	bool Compile(GLuint& loadedShader, const char* source);

	void AttachCompiledShader(GLuint& compiledShader);

	bool Link();

    GLuint shaderProgram;

};

#endif
