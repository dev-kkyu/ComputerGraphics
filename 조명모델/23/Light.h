#pragma once
#include "Header.h"

class Light
{
	const glm::mat4 Unit;
	glm::mat4 Change;
	glm::vec3 Color;
public:
	Light();

	void Update();
	void Draw();
};

