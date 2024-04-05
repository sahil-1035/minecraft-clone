#include "VertexBuffer.h"




VertexBuffer::VertexBuffer()
{}
VertexBuffer::VertexBuffer(unsigned int size, GLenum bufferType, unsigned int arraySize, const void* array)
{
	Init(size, bufferType, arraySize, array);
}
VertexBuffer::~VertexBuffer()
{
	Destroy();
}
void VertexBuffer::Destroy()
{
	glDeleteVertexArrays(_size, &_vertexBuffer);
	glDeleteBuffers(_size, &_vertexBuffer);
}

void VertexBuffer::Init(unsigned int size, GLenum bufferType, unsigned int arraySize, const void* array)
{
	_size = size;
	_bufferType = bufferType;
	glGenBuffers(size, &_vertexBuffer);
	glBindBuffer(_bufferType, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, arraySize, array, GL_STATIC_DRAW); 
}

void VertexBuffer::Bind()
{
	glBindBuffer(_bufferType, _vertexBuffer);
}

void VertexBuffer::UnBind()
{
}
