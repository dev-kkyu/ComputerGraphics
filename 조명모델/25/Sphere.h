#pragma once
#include "Header.h"

class Sphere
{
	GLUquadricObj* qobj;

	const glm::mat4 Unit;

	float rotY, rotZ;

	float nowZAngle;

	float sphScale;
	float CircleScale;
	float Speed;
	glm::vec3 Color;
	glm::mat4 Change;

	glm::mat4 CircleAngle;
	glm::mat4 CircleChange;
public:
	Sphere(float CircleScale, float sphScale, float Speed, glm::vec3 Color, float rotY, float rotZ);

	void Update();
	void Draw();
};

