#pragma once

#include "include/glm/glm.hpp"

struct Camera
{
	glm::vec3 pos;
	glm::vec3 up;
	glm::vec3 dir, right;
	float horizontalAngle, verticalAngle;
	float AspectRatio;
};
