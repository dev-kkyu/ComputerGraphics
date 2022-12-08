#pragma once
#include "Header.h"
class Object
{
protected:
	const glm::mat4 Unit;
	glm::mat4 Rotate;
	glm::mat4 Change;
	glm::vec3 Color;

public:
	Object();

	virtual void FixUpdate() = 0;
	void Update();
	virtual void Draw() = 0;
};

