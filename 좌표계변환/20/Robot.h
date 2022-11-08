#pragma once

#include "Head.h"
#include "Nose.h"
#include "Body.h"
#include "Arm.h"
#include "Leg.h"

#include "Block.h"

extern Block block1, block2;

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

	const int VELOCITY;
	const int MASS;

	int Vel;

public:
	Robot();
	~Robot();

	void Jump();
	void draw();
	void update(unsigned char key);

	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	float getBottom();
	float getTop();
};

bool isCollide(Robot r1, Block r2);
bool isCollide2D(Robot r1, Block r2);