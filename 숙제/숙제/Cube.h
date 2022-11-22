#pragma once
#include "Header.h"

class Cube
{
private:
	const glm::mat4 Unit;
	glm::mat4 Change;

	glm::vec3 Position;			//y�� 0���� ����
	glm::vec3 Color;

	float Xscale;
	float Zscale;

	float Yscale;		//�ִϸ��̼� ť���� ����
	int Direction;		//�ִϸ��̼� �ö󰥰��� ����������

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

