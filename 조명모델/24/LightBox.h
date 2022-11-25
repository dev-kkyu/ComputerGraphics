#pragma once
#include "Header.h"

class LightBox
{
	const glm::mat4 Unit;
	glm::mat4 Change;

public:
	LightBox();

	void Update();
	void Draw();
};

