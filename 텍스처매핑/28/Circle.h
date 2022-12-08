#pragma once
#include "Object.h"

class Circle : public Object
{
	GLUquadricObj* qobj;
public:
	Circle();

	void FixUpdate() override;
	void Draw() override;
};

