#pragma once
#include "Header.h"
class Object
{
protected:
	const glm::mat4 Unit;
	float scale;

	glm::mat4 Change;

	glm::vec3 Color;
	glm::vec3 Position;

public:
	Object();
	virtual ~Object();
	Object(float scale);

	virtual void FixUpdate() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void Drop();
};

