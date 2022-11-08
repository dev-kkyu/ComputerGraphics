#pragma once

#include "Header.h"

class Block
{
	glm::vec3 TopMiddlePoint;

public:
	Block();

	void draw();

	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	float getBottom();
	float getTop();
};

