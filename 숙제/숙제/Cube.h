#pragma once
#include "Header.h"

class Cube
{
private:
	const glm::mat4 Unit;
	glm::mat4 Change;

	glm::vec3 Position;			//y는 0으로 고정
	glm::vec3 Color;

	float Xscale;
	float Zscale;

	float Yscale;		//애니메이션 큐브의 높이
	int Direction;		//애니메이션 올라갈건지 내려갈건지

	float speed;
	float maxScale;

	bool isRender;

public:
	Cube();
	Cube(glm::vec3 Position, float Xscale, float Zscale);
	~Cube();


	void FixUpdate();
	void Update();
	void Draw();

	void SetRender(bool isRender);
	bool GetRender();

	float getLeft();
	float getRight();
	float getBehind();
	float getFront();
	float getBottom();
	float getTop();
};

