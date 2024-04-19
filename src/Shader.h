#pragma once

#include "include/GL/glew.h"
#include "utils/Logs.h"

class Shader
{
public:
	Shader();
	Shader(unsigned int noOfShaders);
	~Shader();
	void Init(const unsigned int noOfShaders);
	void LoadShader(GLenum ShaderType, const char* path);
	void GenProgram();
	GLuint* GetProgram();
	void UseProgram();

private:
	unsigned int _createdShaders;
	unsigned int _noOfShaders;
	GLuint* _shaders;
	GLuint _program;

	Logger _log;
};
