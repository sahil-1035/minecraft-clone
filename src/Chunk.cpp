#include "Chunk.h"

#include <cmath>
#include <random>
#include <filesystem>
#include <sstream>

#include "utils/Timer.h"
#include "utils/File.h"
#include "utils/Logs.h"

#include "blkBitOperations.h"

#include "include/FastNoiseLite.h"


Chunk::Chunk()
{}
Chunk::~Chunk()
{
	/*if (_fileLoad.joinable())
		_fileLoad.join();*/
}
Chunk::Chunk(glm::vec2 pos)
{
	Init(pos);
}
void Chunk::Init(glm::vec2 pos)
{
	_pos = pos;
	_loaded = false;
	std::stringstream tmpss;
	tmpss << "CHUNK ( " << pos.x << ", " << pos.y << " )";
	_log.Init(tmpss.str());
	for(unsigned int i = 0; i < 6; i++)
		surroundingChunks[i] = NULL;
}

void Chunk::GenTree(glm::ivec3 p)
{
	for(unsigned int i =0; i < 5; i++)
		AddBlock(p+glm::ivec3(0,0,i), BlockType::WOOD, 0);

	for(unsigned int z = 5; z < 10; z++)
	{
		if(z<8)
		for(int x = -2; x < 3; x++)
		{
			srand(x);
			int r1 = (x==-2 || x==3) ? rand()%2-2 : -2;
			int r2 = -r1+1;
			for(int y = r1; y < r2; y++)
			{
				AddBlock(p+glm::ivec3(x,y,z), BlockType::LEAF, 1);
			}
		}
		else
		for(int x = -1; x < 1; x++)
		{
			srand(x);
			int r1 = (x==-1 || x==1) ? rand()%2-1 : -1;
			int r2 = -r1+1;
			for(int y = r1; y < r2; y++)
			{
				AddBlock(p+glm::ivec3(x,y,z), BlockType::LEAF, 1);
			}
		}
	}
}

void Chunk::GenChunk()
{
	// Checks if the Chunk has already been generated
	std::stringstream rootFilePath;
	rootFilePath << "data/" << _pos.x << "x" << _pos.y;
	if(std::filesystem::exists(rootFilePath.str()))
	{
		//_fileLoad = std::thread(&Chunk::LoadChunk, this);
		//_fileLoad.join();
		_log[INFO] << "Loading Chunk\n";
		LoadChunk();
		SetUpGPU();
		//_loaded = true;
		return;
	}

	_log[INFO] << "Generating Chunk\n";
	float freq = 50;
	long unsigned int seed = 422;

	FastNoiseLite fn(seed);
	fn.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	srand(_pos.x<<21|_pos.y);

	for(unsigned int x = 0; x < 32; x++)
		for(unsigned int y = 0; y < 32; y++)
		{
			int z = 17.0*fn.GetNoise((float)(32*_pos.x+x), (float)(32*_pos.y+y))+17;
			z += fn.GetNoise((float)(32*_pos.x+x)*10, (float)(32*_pos.y+y)*10);
			z += 40.0*fn.GetNoise((float)(32*_pos.x+x)/30, (float)(32*_pos.y+y)/30);
			//z += 100.0*fn.GetNoise((float)(32*_pos.x+x)/50, (float)(32*_pos.y+y)/50);
			glm::ivec3 p = glm::ivec3(x,y,z);

			if(z<=11)
			{
				AddBlock(glm::ivec3(x,y,10), BlockType::WATER, 0);
				continue;
				for(int i = 0; i < 10; i++)
				{
					AddBlock(glm::ivec3(x,y,i), BlockType::STONE, 0);
				}
				continue;
			}

			//for(int i = 0; i < z-3; i++)
			for(int i = z-3; i < z; i++)
			{
				p = glm::ivec3(x,y,i);
				AddBlock(p, BlockType::STONE, 0);
			}
			AddBlock(p, BlockType::GRASS, 0);
			AddBlock(p+glm::ivec3(0,0,-1), BlockType::GRASS, 0);
			if((int)(100.0f*fn.GetNoise((float)(32*_pos.x+x)*10, (float)(32*_pos.y+y)*10))%100 == 1 && p.x>2 && p.y>2 && 32-p.x>2 && 32-p.y>2)
				GenTree(p);
		}
	ConstructVerticies();
	SetUpGPU();

	SaveChunk();
}

void Chunk::ConstructVerticies()
{
	if(_loaded)
		return;
	Timer constvert(&_log, "Construct Verticies");
	for(const auto&pair:_blocks)
	{
		int mapKey = pair.first;
		glm::vec3 tmpPos = GetBlkPos(mapKey);
		int blktype = GetType(_blocks[mapKey]);
		std::vector<glm::vec3> Face[6];

		auto up    = !BlockExists(tmpPos, UP);
		auto down  = !(BlockExists(tmpPos, DOWN) | GetBlkPos(mapKey).z==0);
		auto north = !BlockExists(tmpPos, NORTH);
		auto south = !BlockExists(tmpPos, SOUTH);
		auto east  = !BlockExists(tmpPos, EAST);
		auto west  = !BlockExists(tmpPos, WEST);
		for(int i = -1; i < 2; i+=2)
			for(int j = -1; j < 2; j+=2)
			{
				if(up)
				{
					Face[UP].push_back(tmpPos + glm::vec3(0.5f * (float)i, 0.5f * (float)j, 0.5f));
					Face[UP].push_back(glm::vec3((i+1)/60.0f,(j+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
				}
				if(false)
				{
					Face[DOWN].push_back(tmpPos + glm::vec3(0.5f * (float)j, 0.5f * (float)i, -0.5f));
					Face[DOWN].push_back(glm::vec3((j+1+10)/60.0f, (i+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
				}
				if(north)
				{
					Face[NORTH].push_back(tmpPos + glm::vec3(0.5f * (float)j, 0.5f, 0.5f * (float)i));
					Face[NORTH].push_back(glm::vec3(1/30.0f + (j+1)/60.0f,-(i+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
				}
				if(south)
				{
					Face[SOUTH].push_back(tmpPos + glm::vec3(0.5f * (float)i, -0.5f, 0.5f * (float)j));
					Face[SOUTH].push_back(glm::vec3(2/30.0f + (i+1)/60.0f,-(j+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
				}
				if(east)
				{
					Face[EAST].push_back(tmpPos + glm::vec3(0.5f,  0.5f * (float)i, 0.5f * (float)j));
					Face[EAST].push_back(glm::vec3(3/30.0f + (i+1)/60.0f,-(j+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
				}
				if(west)
				{
					Face[WEST].push_back(tmpPos + glm::vec3(-0.5f, 0.5f * (float)j, 0.5f * (float)i));
					Face[WEST].push_back(glm::vec3(4/30.0f + (j+1)/60.0f,-(i+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
				}
			}
	
		for(unsigned int i = 0; i < 6; i++)
			for(unsigned int j = 0; j < Face[i].size(); j++)
			{
				_verticies.push_back(Face[i][j].x);
				_verticies.push_back(Face[i][j].y);
				_verticies.push_back(Face[i][j].z);
			}
	}
	for(unsigned int i = 0; i < _verticies.size()/4; i++)
	{
		_indecies.push_back(4 * i + 0);
		_indecies.push_back(4 * i + 1);
		_indecies.push_back(4 * i + 2);

		_indecies.push_back(4 * i + 2);
		_indecies.push_back(4 * i + 1);
		_indecies.push_back(4 * i + 3);
	}
	for(const auto&pair:t_blocks)
	{
		glm::vec3 tmpPos = GetBlkPos(pair.first);
		int mapKey = pair.first;
		int blktype = GetType(t_blocks[mapKey]);
		std::vector<glm::vec3> Face[6];
		for(int i = -1; i < 2; i+=2)
			for(int j = -1; j < 2; j+=2)
			{
				Face[UP].push_back(tmpPos + glm::vec3(0.5f * (float)i, 0.5f * (float)j, 0.5f));
				Face[UP].push_back(glm::vec3((i+1)/96.0f,(j+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
				Face[DOWN].push_back(tmpPos + glm::vec3(0.5f * (float)j, 0.5f * (float)i, -0.5f));
				Face[DOWN].push_back(glm::vec3((j+1+10)/96.0f, (i+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
				Face[NORTH].push_back(tmpPos + glm::vec3(0.5f * (float)j, 0.5f, 0.5f * (float)i));
				Face[NORTH].push_back(glm::vec3(1/48.0f + (j+1)/96.0f,-(i+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
				Face[SOUTH].push_back(tmpPos + glm::vec3(0.5f * (float)i, -0.5f, 0.5f * (float)j));
				Face[SOUTH].push_back(glm::vec3(2/48.0f + (i+1)/96.0f,-(j+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
				Face[EAST].push_back(tmpPos + glm::vec3(0.5f,  0.5f * (float)i, 0.5f * (float)j));
				Face[EAST].push_back(glm::vec3(3/48.0f + (i+1)/96.0f,-(j+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
				Face[WEST].push_back(tmpPos + glm::vec3(-0.5f, 0.5f * (float)j, 0.5f * (float)i));
				Face[WEST].push_back(glm::vec3(4/48.0f + (j+1)/96.0f,-(i+1)/2, 0) + glm::vec3(blktype/5.0f, 0, 0));
			}
		for(unsigned int i = 0; i < 6; i++)
			for(unsigned int j = 0; j < Face[i].size(); j++)
			{
				t_verticies.push_back(Face[i][j].x);
				t_verticies.push_back(Face[i][j].y);
				t_verticies.push_back(Face[i][j].z);
			}
	}
	for(unsigned int i = 0; i < t_verticies.size()/4; i++)
	{
		t_indecies.push_back(4 * i + 0);
		t_indecies.push_back(4 * i + 1);
		t_indecies.push_back(4 * i + 2);

		t_indecies.push_back(4 * i + 2);
		t_indecies.push_back(4 * i + 1);
		t_indecies.push_back(4 * i + 3);
	}
}


inline void Chunk::AddBlock(glm::ivec3 p, BlockType type, bool transparent)
{
	if(transparent)
		t_blocks[GetBitField(p)] = GenBitField(p, type, transparent);
	else
		_blocks[GetBitField(p)] = GenBitField(p, type, transparent);
}

inline bool Chunk::BlockExists(glm::ivec3 p, Direction d)
{
	if(p.x == 0 && d == Direction::WEST && surroundingChunks[WEST] != NULL)
		surroundingChunks[WEST]->BlockExists(glm::ivec3(31, p.y, p.z), WEST);

	else if(p.x == 31 && d == Direction::EAST && surroundingChunks[EAST] != NULL)
		surroundingChunks[EAST]->BlockExists(glm::ivec3(0, p.y, p.z), EAST);

	else if(p.y == 0 && d == Direction::SOUTH && surroundingChunks[SOUTH] != NULL)
		surroundingChunks[SOUTH]->BlockExists(glm::ivec3(p.x, 31, p.z), SOUTH);

	else if(p.y == 31 && d == Direction::NORTH && surroundingChunks[NORTH] != NULL)
		surroundingChunks[NORTH]->BlockExists(glm::ivec3(p.x,  0, p.z), NORTH);

	return _blocks.count(GetBitField(p + GetVec(d)));
}


void Chunk::DrawOpaque()
{
	if(!IsLoaded())
		return;
	glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

	vao.Bind();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	//glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR)
	{}
	glDrawElements(GL_TRIANGLES, _indecies.size(), GL_UNSIGNED_INT, 0);
	vao.UnBind();
    if(glGetError() == GL_INVALID_OPERATION)
		SetUpGPU();
}

void Chunk::DrawTransparent()
{
	if(!IsLoaded())
		return;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	t_vao.Bind();
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR)
	{}
	glDrawElements(GL_TRIANGLES, t_indecies.size(), GL_UNSIGNED_INT, 0);
	t_vao.UnBind();
	if(glGetError() == GL_INVALID_OPERATION)
		SetUpGPU();
}

glm::ivec2 Chunk::GetPos() const
{
	return _pos;
}

void Chunk::ForceLoad()
{
	/*if(!_fileLoad.joinable())
		return;
	_fileLoad.join();
	SetUpGPU();
	_loaded = true;*/

}

bool Chunk::IsLoaded()
{
	if(_loaded)
		return true;
	//else if(!_fileLoad.joinable())
	{
		_log[INFO] << "GPU Reset initiated.\n";
		SetUpGPU();
		_loaded = true;
		return true;
	}
	return false;
}

void Chunk::SetUpGPU()
{
	if(_loaded)
	{
		vao.Bind();
		vb.Destroy();
		ib.Destroy();
		vao.Destroy();
		vao.UnBind();
		t_vao.Bind();
		t_vb.Destroy();
		t_ib.Destroy();
		t_vao.Destroy();
		t_vao.UnBind();
	}

	vao.Init(1);
	vb.Init(1, GL_ARRAY_BUFFER, _verticies.size()*sizeof(_verticies[0]), _verticies.data());
	ib.Init(1, _indecies.size()*sizeof(_indecies[0]), _indecies.data());
	vao.AttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	vao.AttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(GLfloat)));
	vao.UnBind();


	t_vao.Init(1);
	t_vb.Init(1, GL_ARRAY_BUFFER, t_verticies.size()*sizeof(t_verticies[0]), t_verticies.data());
	t_ib.Init(1, t_indecies.size()*sizeof(t_indecies[0]), t_indecies.data());
	t_vao.AttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	t_vao.AttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(GLfloat)));
	t_vao.UnBind();
}

void Chunk::SaveChunk()
{
	// File Path for the root dir of the chunk
	char rootFilePath[30];
	sprintf(rootFilePath, "data/%dx%d", _pos.x, _pos.y);
	std::filesystem::create_directories(rootFilePath);

	char v_FilePath[30], i_FilePath[30], b_FilePath[30];
	char  t_v_FilePath[30], t_i_FilePath[30], t_b_FilePath[30];

	// File Writing for opaque objects
	sprintf(v_FilePath, "%s/verticies.dat", rootFilePath);
	sprintf(i_FilePath, "%s/indecies.dat", rootFilePath);
	sprintf(b_FilePath, "%s/blocks.dat", rootFilePath);
	File2::WriteVector<std::vector<GLfloat>>(&_verticies, v_FilePath);
	File2::WriteVector<std::vector<GLuint>>(&_indecies, i_FilePath);
	//File2::WriteUnorderedMap<std::unordered_map<int, int>>(&_blocks, b_FilePath);

	// File Writing for transparant objects
	sprintf(t_v_FilePath, "%s/t_verticies.dat", rootFilePath);
	sprintf(t_i_FilePath, "%s/t_indecies.dat", rootFilePath);
	sprintf(t_b_FilePath, "%s/t_blocks.dat", rootFilePath);
	File2::WriteVector<std::vector<GLfloat>>(&t_verticies, t_v_FilePath);
	File2::WriteVector<std::vector<GLuint>>(&t_indecies, t_i_FilePath);
	//File2::WriteUnorderedMap<std::unordered_map<int, int>>(&t_blocks, t_b_FilePath);
}


void Chunk::LoadChunk()
{
	char rootFilePath[30];
	sprintf(rootFilePath, "data/%dx%d", _pos.x, _pos.y);

	char v_FilePath[30], i_FilePath[30], b_FilePath[30];
	char  t_v_FilePath[30], t_i_FilePath[30], t_b_FilePath[30];

	sprintf(v_FilePath, "%s/verticies.dat", rootFilePath);
	sprintf(i_FilePath, "%s/indecies.dat", rootFilePath);
	sprintf(b_FilePath, "%s/blocks.dat", rootFilePath);
	File2::LoadVector<std::vector<GLfloat>>(&_verticies, v_FilePath);
	File2::LoadVector<std::vector<GLuint>>(&_indecies, i_FilePath);
	//File2::LoadUnorderedMap<std::unordered_map<int, int>>(&_blocks, b_FilePath);

	sprintf(t_v_FilePath, "%s/t_verticies.dat", rootFilePath);
	sprintf(t_i_FilePath, "%s/t_indecies.dat", rootFilePath);
	sprintf(t_b_FilePath, "%s/t_blocks.dat", rootFilePath);
	File2::LoadVector<std::vector<GLfloat>>(&t_verticies, t_v_FilePath);
	File2::LoadVector<std::vector<GLuint>>(&t_indecies, t_i_FilePath);
	//	File2::LoadUnorderedMap<std::unordered_map<int, int>>(&t_blocks, t_b_FilePath);
}
