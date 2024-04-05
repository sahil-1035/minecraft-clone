#include "VertexArray.h"





VertexArray::VertexArray()
{}
VertexArray::VertexArray(unsigned int size)
{
	Init(size);
}
VertexArray::~VertexArray()
{
	Destroy();
}
void VertexArray::Destroy()
{
	glDeleteVertexArrays(_size, &_vertexArray);
}

void VertexArray::Init(unsigned int size)
{
	_size = size;
	glGenVertexArrays(size, &_vertexArray);
	glBindVertexArray(_vertexArray);
}

void VertexArray::Bind()
{
	glBindVertexArray(_vertexArray);
}

void VertexArray::UnBind()
{
	glBindVertexArray(0);
}

void VertexArray::AttribPointer(GLuint location, GLint lenght, GLenum dataType, GLboolean normalised, GLsizei totalLenghtinBytes, const void* stride)
{
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, lenght, dataType, normalised, totalLenghtinBytes, stride);
}
