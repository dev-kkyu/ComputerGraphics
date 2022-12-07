#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>
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
extern GLuint cubeVAO, equilateralTriVAO, floorVAO, sphereVAO;
extern GLuint floorTexture;
extern GLuint shaderID; //--- 세이더 프로그램 이름

extern vector<glm::vec3> cubevertex;
extern vector<glm::vec3> spherevertex;

extern bool isDepTest;

extern float L_RotYAngle;
extern int isLRotate;
extern int FractalNum;
extern bool isDrowSnow;

extern int isCRotate;
extern float cameraRotAngle;
