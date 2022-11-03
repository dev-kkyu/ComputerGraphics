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

public:
	Robot();

	void draw();
	void update(unsigned char key);
};

