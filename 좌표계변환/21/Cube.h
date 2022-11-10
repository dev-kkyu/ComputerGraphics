#pragma once
#include "Object.h"

class Cube : public Object
{
public:
	Cube();
	Cube(float scale, float zPos);
	~Cube();


	void FixUpdate();
	void Update();
	void Draw();
};

