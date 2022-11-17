#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <random>
#include <utility>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
//#include <gl/glm/gtc/matrix_transform.hpp>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;


extern int TimerValue;


extern GLint winWidth, winHeight;
extern GLuint VAO, FVAO, rectVAO;
extern GLuint shaderID; //--- 세이더 프로그램 이름

extern bool isDepTest;

extern float C_movX, C_movZ;
extern float C_RotYAngle;
extern int isCRevolution;
extern bool isUpDownMove;

extern int personView;

extern pair<float, float> MouseAngle;

extern bool isW, isA, isS, isD;

extern bool isRobotDraw;
extern bool isMoveStopNDown;

extern float MoveSpeed;

extern float robotScale;