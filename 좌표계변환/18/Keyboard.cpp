#include "Keyboard.h"

int isTransZ = 0;
int isRotY = 0;
int isRotX = 0;

float movCX = 0;
float movCZ = 0;

int isCRotation = 0;
int isCRevolutionW = 0;
int isCRevolutionLok = 0;

GLvoid Keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'b':
		isTransZ != 1 ? isTransZ = 1 : isTransZ = 0;
		break;
	case 'B':
		isTransZ != -1 ? isTransZ = -1 : isTransZ = 0;
		break;
	case 'm':
		isRotY != 1 ? isRotY = 1 : isRotY = 0;
		break;
	case 'M':
		isRotY != -1 ? isRotY = -1 : isRotY = 0;
		break;
	case 't':
		isRotX != 1 ? isRotX = 1 : isRotX = 0;
		break;
	case 'T':
		isRotX != -1 ? isRotX = -1 : isRotX = 0;
		break;
	case 'z':
		movCZ += 0.05;
		break;
	case 'Z':
		if (movCZ + 1.f > 0.1)
			movCZ -= 0.05;
		break;
	case 'x':
		movCX += 0.05;
		break;
	case 'X':
		movCX -= 0.05;
		break;
	case 'y':	//자전
		isCRotation != 1 ? isCRotation = 1 : isCRotation = 0;
		break;
	case 'Y':
		isCRotation != -1 ? isCRotation = -1 : isCRotation = 0;
		break;
	case 'r':	//공전(월드Y축기준)
		isCRevolutionW != 1 ? isCRevolutionW = 1 : isCRevolutionW = 0;
		break;
	case 'R':
		isCRevolutionW != -1 ? isCRevolutionW = -1 : isCRevolutionW = 0;
		break;
	case 'a':	//공전(카메라시선?보는목적지?Y축기준)
		isCRevolutionLok != 1 ? isCRevolutionLok = 1 : isCRevolutionLok = 0;
		break;
	case 'A':
		isCRevolutionLok != -1 ? isCRevolutionLok = -1 : isCRevolutionLok = 0;
		break;
	case 's':
	case 'S':
		isTransZ = 0;
		isRotY = 0;
		isRotX = 0;
		isCRotation = 0;
		isCRevolutionW = 0;
		isCRevolutionLok = 0;
		break;
	case 'c':
	case 'C':
		isTransZ = 0;
		isRotY = 0;
		isRotX = 0;
		isCRotation = 0;
		isCRevolutionW = 0;
		isCRevolutionLok = 0;

		movCX = movCZ = transZ = rotY = rotX = CAngle_Rotation = CAngle_RevolutionW = CAngle_RevolutionLok = 0.f;
		break;
	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;
	case 'p': {
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f); //--- 투영 공간을 [-100.0, 100.0] 공간으로 설정

		unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
			break;
	case 'P': {
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0)); //--- 공간을 약간 뒤로 미뤄줌
		unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
			break;
	case 'v': {
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 50.f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -3.0)); //--- 공간을 약간 뒤로 미뤄줌
		unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

		glm::mat4 view(1.0f);
		unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- 뷰잉 변환 설정
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	}
			break;
	case 'h':
		isDepTest = !isDepTest;
		break;
	}

	glutPostRedisplay();
}