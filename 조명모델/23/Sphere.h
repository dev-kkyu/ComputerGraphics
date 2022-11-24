#pragma once
#include "Header.h"

class Sphere
{
	const glm::mat4 Unit;
	float XPostion;
	glm::vec3 scale;
	glm::vec3 Color;
	glm::mat4 Change;
public:
	Sphere(float XPostion, glm::vec3 Color, float Scale);

	void Update();
	void Draw();
};

