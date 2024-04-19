#pragma once

#include "Shader.h"
#include "Camera.h"
#include "Chunk.h"
#include "utils/Logs.h"

#include <vector>
#include <unordered_map>




class ChunkManager
{
public:
	ChunkManager();
	ChunkManager(unsigned int renderDistance);
	~ChunkManager();
	void Init(unsigned int renderDistance);

	void GenerateChunks(glm::vec3 playerPos);
	void MapSurroundingChunks(glm::ivec2 chunkPos);

	void UpdateChunks(glm::vec3 playerPos);

	bool AddChunk(glm::ivec2 chunkPos);
	void DrawChunks(Camera* cam, Shader* shader, float aspectRatio, glm::vec3 playerPos, glm::vec2 lookAtVec);

	void RefreshGPU();

	Chunk* GetChunk(glm::ivec2 pos);
	int GetKey(glm::ivec2 pos);
private:
	unsigned int _renderDistance;
	//std::vector<Chunk*> v_chunks;
	glm::vec3 _playerPos;

	std::unordered_map<int, unsigned int> _chunkIndex;
	//std::unordered_map<int, Chunk*> _chunk;
	//std::vector<Chunk*> _chunks;

	Logger _log;

	bool _firstload;

	unsigned int _chunksLen;
	Chunk* _chunks[5000];
};
