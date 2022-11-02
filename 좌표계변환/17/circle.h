#pragma once
#include "header.h"

extern GLuint cVAO;
extern GLuint VAO, VBO[2];
extern GLuint shaderID; //--- 세이더 프로그램 이름
extern vector<glm::ivec3> face;
extern vector<glm::vec3> vcolor[3];
extern vector<glm::vec3> rndColor;

void SetColor(int index);

class Circle {
	float Angle;
	float inAngle;	//공전속도 조절
	float outAngle;	//공전의 공전 속도조절
	unsigned int modelLocation;

	void outCircle(float spinAngle, float speed);

public:
	Circle();
	Circle(float Angle);

	void draw();
	void addAngle(float, float);
};