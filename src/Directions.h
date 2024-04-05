#pragma once


#include "include/glm/glm.hpp"


enum Direction {UP, DOWN, NORTH, SOUTH, EAST, WEST};
inline glm::ivec3 GetVec(Direction d)
{
	switch(d)
	{
		case UP:
			return glm::ivec3(0, 0, 1);
			break;
		case DOWN:
			return glm::ivec3(0, 0,-1);
			break;
		case NORTH:
			return glm::ivec3(0, 1, 0);
			break;
		case SOUTH:
			return glm::ivec3(0,-1, 0);
			break;
		case EAST:
			return glm::ivec3(1, 0, 0);
			break;
		case WEST:
			return glm::ivec3(-1, 0, 0);
			break;
	}
	return glm::ivec3(0, 0, 1);
}
