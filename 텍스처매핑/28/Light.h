#pragma once
#include "Object.h"

class Light : public Object
{
public:
	Light();

	void FixUpdate() override;
	void Draw() override;
};

