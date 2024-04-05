#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

#include <iostream>


Texture::Texture()
{}
Texture::Texture(unsigned int count)
{
	Init(count);
}
Texture::~Texture()
{
	glDeleteTextures(_count, &_textures);
}

void Texture::Init(unsigned int count)
{
	_count = count;
	glGenTextures(count, &_textures);
	/* stbi_set_flip_vertically_on_load(true); */

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Load(const char* texturePath)
{
	int x;
	int y;
	int channels;
	unsigned char* data = stbi_load(texturePath, &x, &y, &channels, 0);
	Bind();
	if (!data)
		std::cout<<"[ERROR]  "<<texturePath<<"  could not be loaded"<<std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, _textures);
}
void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
