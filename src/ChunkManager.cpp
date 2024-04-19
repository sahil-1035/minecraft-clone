#include "ChunkManager.h"

//#include <bits/stdc++.h>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <cmath>

#include "include/GLFW/glfw3.h"

#include "include/glm/glm.hpp"
#include "include/glm/gtx/transform.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtx/quaternion.hpp"

#include "Chunk.h"
#include "Directions.h"

#include "utils/Timer.h"

unsigned int Distance(glm::vec2* p1, glm::vec2* p2);


ChunkManager::ChunkManager()
{}
ChunkManager::ChunkManager(unsigned int renderDistance)
{
	Init(renderDistance);
}
ChunkManager::~ChunkManager()
{
	for(unsigned int i = 0; i <_chunksLen; i++)
		delete _chunks[i];
}

void ChunkManager::Init(unsigned int renderDistance)
{
	_renderDistance = renderDistance;
	_chunksLen = 4*_renderDistance*_renderDistance + 10;
	for(unsigned int i = 0; i <_chunksLen; i++)
		_chunks[i] = NULL;
	_firstload = true;
	_log.Init("Chunk Manager");
}

bool ChunkManager::AddChunk(glm::ivec2 chunkPos)
{
	int key = GetKey(chunkPos);
	if(_chunkIndex.find(key) != _chunkIndex.end())
		return false;

	Chunk* newChunk = new(Chunk);
	for(unsigned int i = 0; i <_chunksLen; i++)
		if(_chunks[i] == NULL)
		{
			_chunks[i] = newChunk;
			_chunkIndex[key] = i;
			newChunk->Init(chunkPos);
			newChunk->GenChunk();
			return true;
		}
	return false;
}


void ChunkManager::GenerateChunks(glm::vec3 playerPos)
{
	_playerPos = playerPos;
	int xPos = (int)playerPos.x;
	int yPos = (int)playerPos.y;
	AddChunk(glm::vec2(xPos/32,yPos/32));
	int sp = 1;
	if(!_firstload)
		sp = _renderDistance - 1;
	for(int layer = 1; layer < _renderDistance; layer++)
	{
		for(int x = -layer; x < layer + 1; x+=2*layer)
			for(int y = -abs(x); y < abs(x) + 1; y+=1)
				if(AddChunk(glm::ivec2(int(x+xPos/32),int(y+yPos/32))))
					return;

		for(int y = -layer; y < layer + 1; y+=2*layer)
			for(int x = -abs(y); x < abs(y) + 1; x+=1)
				if(AddChunk(glm::ivec2(int(x+xPos/32),int(y+yPos/32))))
					return;
	}
	_firstload = false;
}


void ChunkManager::UpdateChunks(glm::vec3 playerPos)
{
	bool chunkRegen = false;
	_playerPos = playerPos;

	glm::ivec2 pPos;
	std::vector<unsigned int> deleteList;
	for(unsigned int i = 0; i < _chunksLen; i++)
	{
		if(_chunks[i] == NULL)
			continue;
		pPos = glm::ivec2(playerPos.x/32, playerPos.y/32);
		unsigned int dist = glm::length((glm::vec2)(_chunks[i]->GetPos() - pPos));
		if(dist > _renderDistance * sqrt(2))
		{
			//std::cout<<"Deleting Chunk "<<_chunks[i]->GetPos().x<<","<<_chunks[i]->GetPos().y<<"\t Dist "<<dist<<std::endl;
			deleteList.push_back(i);
			chunkRegen = true;
		}
	}
	for(unsigned int i = 0; i < deleteList.size(); i++)
	{
		_chunkIndex.erase(GetKey(_chunks[deleteList[i]]->GetPos()));
		delete _chunks[deleteList[i]];
		_chunks[deleteList[i]] = NULL;
	}
	if(true)
		GenerateChunks(playerPos);
}



Chunk* ChunkManager::GetChunk(glm::ivec2 pos)
{
	return _chunks[GetKey(pos)];
}
int ChunkManager::GetKey(glm::ivec2 pos)
{
	return ((short int)pos.x)<<16 | (short int)pos.y;
}




void ChunkManager::DrawChunks(Camera* cam, Shader* shader, float aspectRatio, glm::vec3 playerPos, glm::vec2 lookAtVec)
{
	/*unsigned int count = 0;
	for (unsigned int i = 0; i < _chunksLen; i++)
		if(_chunks[i] != NULL)
			if (_chunks[i]->IsLoaded())
				count++;
	std::cout << "chunks loaded " << count << std::endl;*/
	GLint MVPuniLoc = glGetUniformLocation(*(shader->GetProgram()), "MVP");
	GLint muniLoc = glGetUniformLocation(*(shader->GetProgram()), "m");
	GLint vuniLoc = glGetUniformLocation(*(shader->GetProgram()), "v");
	GLint puniLoc = glGetUniformLocation(*(shader->GetProgram()), "p");
	GLint t = glGetUniformLocation(*(shader->GetProgram()), "t");
	glm::vec3 EulerAngles;
	glm::quat MyQuaternion;
	glm::mat4 model, view, projection, mvp;
	glm::vec2 chunkPos;
	for(int f = 0; f < 2; f++)
	{
		for(unsigned int i = 0; i < _chunksLen; i++)
		{
			if(_chunks[i] == NULL)
				continue;

			chunkPos = _chunks[i]->GetPos();
			if(glm::dot(glm::normalize(glm::vec2(_chunks[i]->GetPos()*32 - glm::ivec2(playerPos) + glm::ivec2(glm::normalize(lookAtVec)*40.0f))), glm::normalize(lookAtVec)) < 0.6f)
				continue;
			//EulerAngles = glm::vec3(0, glfwGetTime(), (glfwGetTime()));
			EulerAngles = glm::vec3(0, 0, 0);
			MyQuaternion = glm::quat(EulerAngles);

			model = glm::translate(glm::mat4(1.0f), glm::vec3(chunkPos.x*32, chunkPos.y*32, 0*sin(glfwGetTime()+ chunkPos.x*32* chunkPos.y*32))) *
							glm::toMat4(MyQuaternion) *
							glm::scale(glm::vec3(1+0*sin(glfwGetTime())));
			view = glm::lookAt(cam->pos, cam->pos + cam->dir, glm::vec3(0, 0,1));
			projection = glm::perspective(glm::radians(40.0f), aspectRatio, 0.1f, 1000.0f);
	

			glUniformMatrix4fv(muniLoc,1, GL_FALSE, &model[0][0]);
			glUniformMatrix4fv(vuniLoc,1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(puniLoc,1, GL_FALSE, &projection[0][0]);
			glUniform1f(t, glm::length(chunkPos*32.0f - glm::vec2(playerPos)));
			if(f==0)
				_chunks[i]->DrawOpaque();
			else
				_chunks[i]->DrawTransparent();
		}
	}
}


void ChunkManager::RefreshGPU()
{
	for(unsigned int i = 0; i < _chunksLen; i++)
	{
		if(_chunks[i] == NULL)
			continue;
		_chunks[i]->SetUpGPU();
	}
}

void ChunkManager::MapSurroundingChunks(glm::ivec2 chunkPos)
{
	_chunks[GetKey(chunkPos)]->surroundingChunks[NORTH] = _chunks[GetKey(chunkPos + glm::ivec2(GetVec(NORTH)))];
	_chunks[GetKey(chunkPos)]->surroundingChunks[SOUTH] = _chunks[GetKey(chunkPos + glm::ivec2(GetVec(SOUTH)))];
	_chunks[GetKey(chunkPos)]->surroundingChunks[EAST]  = _chunks[GetKey(chunkPos + glm::ivec2(GetVec(EAST)))];
	_chunks[GetKey(chunkPos)]->surroundingChunks[WEST]  = _chunks[GetKey(chunkPos + glm::ivec2(GetVec(WEST)))];
}
