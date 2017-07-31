#include "ComputeShader.h"

ComputeShader::ComputeShader(string shaderFile)
{
    shaderProgram = glCreateProgram();

    if (!LoadShader(shaderFile))
    {
        Logger::LogError("cant load shaders");
    };

    Logger::Log("Compute shader program init correctly", "computeshader.cpp");
}

bool ComputeShader::LoadShader(string fileName)
{
    string fileContent;
    if (!FileHelper::ReadShaderFromFile(fileName, &fileContent))
    {
        return false;
    };

    CompileShader(fileContent);

    return true;
}

bool ComputeShader::CompileShader(string& fileContent)
{
	GLint isCompoiled;
	auto fragmentSourceTemp = fileContent.c_str();
	GLuint fragmentShader = glCreateShader(GL_COMPUTE_SHADER);

	isCompoiled = Compile(fragmentShader, fragmentSourceTemp);

	return isCompoiled;
}

bool ComputeShader::Compile(GLuint &loadedShader, const char *source)
{
    GLint isCompoiled;

    glShaderSource(loadedShader, 1, &source, 0);
    glCompileShader(loadedShader);
    glGetShaderiv(loadedShader, GL_COMPILE_STATUS, &isCompoiled);

    if (!isCompoiled)
    {
        GLint length = 0;
        glGetShaderiv(loadedShader, GL_INFO_LOG_LENGTH, &length);

        vector<char> errorLog(length);
        glGetShaderInfoLog(loadedShader, length, &length, &errorLog[0]);

        glDeleteShader(loadedShader);

        Logger::LogError(&errorLog[0]);
    }

    AttachCompiledShader(loadedShader);
    return isCompoiled;
}

void ComputeShader::AttachCompiledShader(GLuint &compiledShader)
{
    glAttachShader(shaderProgram, compiledShader);
}