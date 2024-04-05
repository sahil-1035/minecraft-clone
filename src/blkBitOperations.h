#pragma once

enum BlockType{GRASS, STONE, WOOD, LEAF, WATER};

#include "include/glm/glm.hpp"

inline glm::ivec3 GetBlkPos(int bl)
{
	glm::ivec3 pos(0);
	pos.x = (bl & (0b111111 << 26)) >> 26;
	pos.y = (bl & (0b111111 << 20)) >> 20;
	pos.z = (bl & (0b11111111 << 12)) >> 12;
	return pos;
}
inline int GetBitField(glm::ivec3 pos)
{
	int bl = 0;
	bl |= pos.x << 26;
	bl |= pos.y << 20;
	bl |= pos.z << 12;
	return bl;
}
inline BlockType GetType(int bl)
{
	return (BlockType)(bl & 0b1111111111);
}
inline int GenBitField(glm::ivec3 pos, BlockType type, bool transparent)
{
	int bl = GetBitField(pos);
	bl |= type;
	bl |= transparent << 11;
	return bl;
}
inline bool istransparent(int bl)
{
	return (bl & 0b100000000000)>>11;
}
