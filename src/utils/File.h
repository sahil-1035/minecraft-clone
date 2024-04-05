#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "../include/GL/glew.h"
#include "Timer.h"

namespace File2
{
	template<typename T>
	void WriteVector(T* vec, char filePath[]);
	template<typename T>
	void LoadVector(T* vec, char filePath[]);

	template<typename T>
	void WriteUnorderedMap(T* umap, char filePath[]);
	template<typename T>
	void LoadUnorderedMap(T* umap, char filePath[]);
}

template <typename T>
void File2::WriteVector(T* vec, char filePath[30])
{
	std::ofstream file(filePath, std::ios::binary | std::ios::out);
	file.write(reinterpret_cast<char*>(vec->data()), vec->size() * sizeof((*vec)[0]));
	file.close();
}

template <typename T>
void File2::LoadVector(T* vec, char filePath[30])
{
	unsigned int numFloats;
	size_t fileSize;
	std::ifstream file(filePath, std::ios::binary | std::ios::in);

	//Getting the File Size
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// Calculate the number of floats in the file
	numFloats = fileSize / sizeof(GLuint);
	vec->resize(numFloats);

	file.read(reinterpret_cast<char*>(vec->data()), fileSize);
	file.close();
}


template <typename T>
void File2::WriteUnorderedMap(T* umap, char filePath[])
{
	std::ofstream file(filePath, std::ios::binary | std::ios::out);
	for (const auto& entry : (*umap))
	{
		int key = entry.first;
		int value = entry.second;

		file.write(reinterpret_cast<char*>(&key), sizeof(key));
		file.write(reinterpret_cast<char*>(&value), sizeof(value));
	}
	file.close();
}
template <typename T>
void File2::LoadUnorderedMap(T* umap, char filePath[])
{
	std::ifstream file(filePath, std::ios::binary | std::ios::in);
	while (true)
	{
		Timer pt("pair time");
		int key;
		int value;
		file.read(reinterpret_cast<char*>(&key), sizeof(key));
		file.read(reinterpret_cast<char*>(&value), sizeof(value));
		(*umap)[key] = value;
		if (file.eof())
			break;
	}
	file.close();
}




class File
{
public:
	File();
	File(const char* filePath);
	~File();
	void Init(const char* filePath);
	std::string ReadFile();
private:
	const char* _filePath;
};
