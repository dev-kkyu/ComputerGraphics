#pragma once
#include "header.h"

extern GLuint cVAO;
extern GLuint VAO, VBO[2];
extern GLuint shaderID; //--- ���̴� ���α׷� �̸�
extern vector<glm::ivec3> face;
extern vector<glm::vec3> vcolor[3];
extern vector<glm::vec3> rndColor;

void SetColor(int index);

class Circle {
	float Angle;
	float inAngle;	//�����ӵ� ����
	float outAngle;	//������ ���� �ӵ�����
	unsigned int modelLocation;

	void outCircle(float spinAngle, float speed);

public:
	Circle();
	Circle(float Angle);

	void draw();
	void addAngle(float, float);
};