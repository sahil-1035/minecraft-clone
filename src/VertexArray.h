#pragma once

#include "include/GL/glew.h"


class VertexArray
{
public:
	VertexArray();
	VertexArray(unsigned int size);
	~VertexArray();
	void Init(unsigned int size);
	void Destroy();
	void Bind();
	void UnBind();
	void AttribPointer(GLuint location, GLint lenght, GLenum dataType, GLboolean normalised, GLsizei totalLenghtinBytes, const void* stride);
private:
	GLuint _vertexArray;
	unsigned int _size;
};
