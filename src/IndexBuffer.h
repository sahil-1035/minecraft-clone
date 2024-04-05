#pragma once

#include "include/GL/glew.h"


class IndexBuffer 
{
public:
	IndexBuffer();
	IndexBuffer(unsigned int size, unsigned int arraySize, const void* array);
	~IndexBuffer();
	void Destroy();
	void Init(unsigned int size, unsigned int arraySize, const void* array);
	void Bind();
	void UnBind();
private:
	GLuint _indexBuffer;
	unsigned int _size;
};
