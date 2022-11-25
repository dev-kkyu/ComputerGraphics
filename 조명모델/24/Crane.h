#pragma once
#include "Header.h"

class Crane
{
	const glm::mat4 Unit;
	glm::mat4 Change;

public:
	Crane();

	void Draw();

	void BottomUpdate();
	void MiddleUpdate();
	void TopUpdate(int i);

};

