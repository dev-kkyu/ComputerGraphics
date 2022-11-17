#pragma once

#include "Header.h"

class RobotParts
{
protected:
	const glm::mat4 Unit;
	glm::mat4 Change;

	glm::vec3 Color;


	glm::vec3 Position;

	float Direction;

public:
	RobotParts();
	virtual ~RobotParts();

	void draw();
	void keyIn(glm::vec3 Position, float Direction);
	virtual void Update() = 0;			//순수 가상함수, 추상클래스

};

