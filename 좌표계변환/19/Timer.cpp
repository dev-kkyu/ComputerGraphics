#include "Timer.h"

int TimerValue = 10;

GLvoid TimerFunction(int value)
{
	if (isTransZ == 1) {
		transZ += 0.005f;
	}
	else if (isTransZ == -1) {
		transZ -= 0.005f;
	}
	if (isRotY == 1) {
		rotY += 1.f;
	}
	else if (isRotY == -1) {
		rotY -= 1.f;
	}
	if (isRotX == 1) {
		if (rotX < 89.999999f)
			rotX += 1.f;
	}
	else if (isRotX == -1) {
		if (rotX > -89.999999f)
			rotX -= 1.f;
	}
	if (isCRotation == 1) {
		CAngle_Rotation += 1.f;
	}
	else if (isCRotation == -1) {
		CAngle_Rotation -= 1.f;
	}
	if (isCRevolutionW == 1) {
		CAngle_RevolutionW += 1.f;
	}
	else if (isCRevolutionW == -1) {
		CAngle_RevolutionW -= 1.f;
	}
	if (isCRevolutionLok == 1) {
		CAngle_RevolutionLok += 1.f;
	}
	else if (isCRevolutionLok == -1) {
		CAngle_RevolutionLok -= 1.f;
	}

	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}