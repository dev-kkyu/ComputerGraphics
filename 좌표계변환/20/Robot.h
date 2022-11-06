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

	const int VELOCITY;
	const int MASS;

	int Vel;

public:
	Robot();

	void Jump();
	void draw();
	void update(unsigned char key);
};

