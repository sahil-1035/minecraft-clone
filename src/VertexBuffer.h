#pragma once

#include "include/GL/glew.h"


class VertexBuffer 
{
public:
	VertexBuffer();
	VertexBuffer(unsigned int size, GLenum bufferType, unsigned int arraySize, const void* array);
	~VertexBuffer();
	void Init(unsigned int size, GLenum bufferType, unsigned int arraySize, const void* array);
	void Destroy();
	void Bind();
	void UnBind();
private:
	GLuint _vertexBuffer;
	GLenum _bufferType;
	unsigned int _size;
};
