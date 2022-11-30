#pragma once
#include "Header.h"

class Snow
{
	float Speed;
	float Scale;
	glm::vec3 Color;
	glm::vec3 Position;
	glm::mat4 Change;

public:
	Snow();

	void Update();
	void Draw();
};

