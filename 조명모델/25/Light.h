#pragma once
#include "Header.h"

class Light
{
	const glm::mat4 Unit;
	glm::mat4 Change;
	glm::vec3 BoxColor;
	glm::vec3 LightColor;

	float distance;
public:
	Light();

	void EditValue(unsigned char key);

	void Update();
	void Draw();
};

