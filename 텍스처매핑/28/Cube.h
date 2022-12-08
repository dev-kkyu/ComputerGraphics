#pragma once
#include "Object.h"

class Cube : public Object
{
public:
	Cube();

	void FixUpdate() override;
	void Draw() override;
};

