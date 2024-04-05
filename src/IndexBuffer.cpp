#include "IndexBuffer.h"




IndexBuffer::IndexBuffer()
{}
IndexBuffer::IndexBuffer(unsigned int size, unsigned int arraySize, const void* array)
{
	Init(size, arraySize, array);
}
IndexBuffer::~IndexBuffer()
{
	Destroy();
}
void IndexBuffer::Destroy()
{
	glDeleteBuffers(_size, &_indexBuffer);
}

void IndexBuffer::Init(unsigned int size, unsigned int arraySize, const void* array)
{
	_size = size;
	glGenBuffers(size, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, arraySize, array, GL_STATIC_DRAW); 
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
}

void IndexBuffer::UnBind()
{
}
