#pragma once
#include "Header.h"
#include "Cube.h"
#include "Robot.h"
#include "LightBox.h"

class World
{
	vector<vector<Cube*>*> CUBE;
	Robot ROBOT;
	LightBox LIGHT;

	float dxObjPos;
	float dzObjPos;

	float dyObjPos;

	pair<float, float> startObjPos;

	const int VELOCITY;
	const int MASS;

	int Vel;

public: 
	World();

	void Jump();

	void Draw();
	void Update();

	void LightUpdate(int Direction);

	void MakeMaze();
	void Camera(int personView);

	void KeyIn(GLuint key);

	bool isCollide(Robot r1, Cube r2);
	bool isCollide2D(Robot r1, Cube r2);
};
