#pragma once
#include "Header.h"

class SierpinskiPyramid
{
	glm::vec3 Color;

public:
	SierpinskiPyramid(glm::vec3 Color);

	void DrawTri(glm::mat4 Change);
	void DrawFace(int fractalNum, glm::mat4 Change);
	void Draw(int fractalNum);

};

