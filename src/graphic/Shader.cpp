#include "Shader.h"

Shader::Shader()
{
}

Shader::Shader(string shaderProgramName, string vertexFileName, string fragmentFileName)
{
	shaderProgram = glCreateProgram();

	if (!LoadVertexShader(shaderProgram, vertexFileName))
	{
		Logger::LogError("cant load shaders");
	};

	if (!LoadFragmentShader(shaderProgram, fragmentFileName))
	{
		Logger::LogError("cant compile vertex shader");
	};

	if (!Link(shaderProgram))
	{
		Logger::LogError("cant link shader");
	};

	Logger::Log("shader program init correctly", "shader.cpp");
}

Shader::~Shader()
{
}

GLint Shader::GetAttributeLocationBy(string name)
{
	return GLint();
}

GLint Shader::GetUniformLocationBy(string name)
{
	return GLint();
}

void Shader::SetUniformMatrix(GLint location, glm::mat4 matrix)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniformMatrix(string name, glm::mat4 matrix)
{
	GLint location = GetUniformLocationBy(name);
	SetUniformMatrix(location, matrix);
}

void Shader::SetAttributeBy(GLint location, string name)
{
}

GLuint Shader::GetShaderProgram()
{
	return shaderProgram;
}

bool Shader::LoadVertexShader(GLuint & shaderProgram, string fileName)
{
	string fileContent;
	if (!FileHelper::ReadShaderFromFile(fileName, &fileContent))
	{
		return false;
	};

	CompileVertexShader(shaderProgram, fileContent);

	return true;
}

bool Shader::LoadFragmentShader(GLuint & shaderProgram, string fileName)
{
	string fileContent;
	if (!FileHelper::ReadShaderFromFile(fileName, &fileContent))
	{
		return false;
	};

	CompileFragmentShader(shaderProgram, fileContent);

	return true;
}

bool Shader::CompileVertexShader(GLuint & shaderProgram, string & fileContent)
{
	GLint isCompoiled;
	auto vertexSourceTemp = fileContent.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	isCompoiled = Compile(shaderProgram, vertexShader, vertexSourceTemp);

	return isCompoiled;
}

bool Shader::CompileFragmentShader(GLuint & shaderProgram, string & fileContent)
{
	GLint isCompoiled;
	auto fragmentSourceTemp = fileContent.c_str();
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	isCompoiled = Compile(shaderProgram, fragmentShader, fragmentSourceTemp);

	return isCompoiled;
}

bool Shader::Compile(GLuint & shaderProgram, GLuint & loadedShader, const char * source)
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

	AttachCompiledShader(shaderProgram, loadedShader);
	return isCompoiled;
}

void Shader::AttachCompiledShader(GLuint & shaderProgram, GLuint & compiledShader)
{
	glAttachShader(shaderProgram, compiledShader);
}

bool Shader::Link(GLuint & shaderProgram)
{
	glLinkProgram(shaderProgram);
	GLint isLinked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);

	return isLinked;
}
