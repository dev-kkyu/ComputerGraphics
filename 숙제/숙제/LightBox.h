#pragma once
#include "Header.h"

class LightBox
{
private:
	const glm::mat4 Unit;
	glm::mat4 Rotate;
	glm::mat4 Change;

	float LightAngle;

	glm::vec3 Position;			//y는 0으로 고정
	glm::vec3 Color;

public:
	LightBox();
	LightBox(glm::vec3 Position);

	void setAngle(float Angle);

	void Update(int Direction);
	void Draw();
};



