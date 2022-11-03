#pragma once

#include "Header.h"

class RobotParts
{
protected:
	const glm::mat4 Unit;
	glm::mat4 Scale;
	glm::mat4 Trans;
	glm::mat4 Rotate;
	glm::mat4 Change;

	glm::vec3 Color;

	glm::vec3 Position;

	float Direction;

public:
	RobotParts();

	void draw();
	void keyIn(unsigned char key);
	virtual void update();

};

