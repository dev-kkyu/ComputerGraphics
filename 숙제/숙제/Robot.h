#pragma once

#include "Head.h"
#include "Nose.h"
#include "Body.h"
#include "Arm.h"
#include "Leg.h"

class Robot
{
	Head head;
	Nose nose;
	Body body;
	Arm armL;
	Arm armR;
	Leg legL;
	Leg legR;

	glm::vec3 Position;
	float Direction;

public:
	Robot();
	Robot(glm::vec3 Position);

	void Draw();
	void Update(float ObjPosX, float ObjPosY, float ObjPosZ);

	void setPos(glm::vec3 Position, float Direction);

	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	float getBottom();
	float getTop();
};
