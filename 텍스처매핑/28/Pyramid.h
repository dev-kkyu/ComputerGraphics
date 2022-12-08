#pragma once
#include "Object.h"

class Pyramid : public Object
{
public:
	Pyramid();

	void FixUpdate() override;
	void Draw() override;
};

