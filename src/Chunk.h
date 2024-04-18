#pragma once

#include "utils/Logs.h"
#define CALLBACK
#include "include/glm/glm.hpp"
#include "include/GL/glew.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <unordered_map>
#include <thread>
#include <mutex>
#include <vector>

#include "blkBitOperations.h"
#include "Directions.h"


class Chunk
{
public:
	Chunk();
	~Chunk();
	Chunk(glm::vec2 pos);
	void Init(glm::vec2 pos);
	void GenChunk();

	void GenTree(glm::ivec3 p);
	glm::ivec2 GetPos() const;
	void ConstructVerticies();

	bool IsLoaded();
	void ForceLoad();

	void DrawOpaque();
	void DrawTransparent();

	void SetUpGPU();

	void SaveChunk();
	void LoadChunk();

	inline void AddBlock(glm::ivec3 p, BlockType type, bool transparent);
	inline bool BlockExists(glm::ivec3 p, Direction d);

	Chunk* surroundingChunks[6];
private:
	std::unordered_map<int, int> _blocks;
	std::unordered_map<int, int> t_blocks;
	glm::ivec2 _pos;

	VertexArray vao;
	VertexBuffer vb;
	IndexBuffer ib;
	std::vector<GLfloat> _verticies;
	std::vector<GLuint> _indecies;

	VertexArray t_vao;
	VertexBuffer t_vb;
	IndexBuffer t_ib;
	std::vector<GLfloat> t_verticies;
	std::vector<GLuint> t_indecies;

	Logger _log;
	//	std::thread _fileLoad;
	bool _loaded;
};




