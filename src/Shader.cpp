#include "Shader.h"

#include <iostream>
#include <vector>

#include "utils/File.h"



Shader::Shader()
{}
Shader::Shader(unsigned int noOfShaders)
{
	Init(noOfShaders);
}

Shader::~Shader()
{
}

void Shader::Init(const unsigned int noOfShaders)
{
	_createdShaders = 0;
	_noOfShaders = noOfShaders;
	_shaders = new GLuint[noOfShaders];
	_log.Init("SHADER");
}

void Shader::LoadShader(GLenum ShaderType, const char* path)
{
	unsigned int index = _createdShaders;
	_shaders[index] = glCreateShader(ShaderType);

	File shaderFile(path);
	std::string code = shaderFile.ReadFile();
	const char* glslCode = code.c_str();
	_log[INFO] << "Compiling shader : " << path << "\n";
	glShaderSource(_shaders[index], 1, &glslCode, NULL);
	glCompileShader(_shaders[index]);

	GLint result;
	int infoLogLength;
	glGetShaderiv(_shaders[index], GL_COMPILE_STATUS, &result);
	glGetShaderiv(_shaders[index], GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0)
	{
		std::vector<char> shaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(_shaders[index], infoLogLength, NULL, &shaderErrorMessage[0]);
		_log[ERROR] << &shaderErrorMessage[0] << "\n";
	}
	_createdShaders += 1;
}	

void Shader::GenProgram()
{
	_log[INFO] << "Linking program" << "\n";
	_program = glCreateProgram();
	for(unsigned int i = 0; i < _noOfShaders; i++)
		glAttachShader(_program, _shaders[i]);
	glLinkProgram(_program);

	GLint result;
	int infoLogLength;
	glGetProgramiv(_program, GL_LINK_STATUS, &result);
	glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0)
	{
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(_program, infoLogLength, NULL, &programErrorMessage[0]);
		_log[ERROR] << &programErrorMessage[0] << "\n";
	}

	for(unsigned int i = 0; i < _noOfShaders; i++)
	{
		glDetachShader(_program, _shaders[i]);
		glDeleteShader(_shaders[i]);
	}
	UseProgram();
}

void Shader::UseProgram()
{
	glUseProgram(_program);
}


GLuint* Shader::GetProgram()
{
	return &_program;
}
