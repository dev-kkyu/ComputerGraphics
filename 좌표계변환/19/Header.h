#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <random>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;


extern int TimerValue;

extern GLint winWidth, winHeight;

extern bool isDepTest;
extern GLuint shaderID; //--- 세이더 프로그램 이름
extern GLuint VAO, crossVAO, floorVAO, rectVAO, rectVBO;

extern float movCX, movCZ;

extern float transZ, rotY, rotX;
extern int isTransZ, isRotY, isRotX;

extern float CAngle_Rotation;
extern float CAngle_RevolutionW;
extern float CAngle_RevolutionLok;

extern int isCRotation;			//자전
extern int isCRevolutionW;		//공전
extern int isCRevolutionLok;

extern float rectData[4][6];
