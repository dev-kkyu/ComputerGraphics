#pragma once
#include "header.h"

extern float movX, movY, movZ;
extern bool isDepTest;
extern GLuint shaderID; //--- 세이더 프로그램 이름

GLvoid Keyboard(unsigned char key, int x, int y);

