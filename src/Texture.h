#pragma once

#include "include/GL/glew.h"


class Texture
{
public:
	Texture();
	Texture(unsigned int count);
	~Texture();
	void Init(unsigned int count);
	void Load(const char* texturePath);
	void Bind();
	void UnBind();
private:
	GLuint _textures;
	unsigned int _count;
};
