#pragma once
#include "Header.h"

class Cube
{
	const glm::mat4 Unit;
	glm::mat4 Change;

	glm::vec3 Color;

	glm::vec3 Position;

public:
	Cube(glm::vec3 Position);

	void Update();
	void Draw();
};

