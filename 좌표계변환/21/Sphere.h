#pragma once
#include "Object.h"

class Sphere : public Object
{
	GLUquadricObj* qobj;

	glm::ivec3 Direction;

public:
	Sphere();
	Sphere(float scale);
	~Sphere();

	void FixUpdate();
	void Update();
	void Draw();
};

