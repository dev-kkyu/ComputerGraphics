#include "Timer.h"

int TimerValue = 10;

GLvoid TimerFunction(int value)
{
	if (isORotateY == 1) {
		O_RotYAngle += 1.f;
	}
	else if (isORotateY == -1) {
		O_RotYAngle -= 1.f;
	}
	if (isORotateX == 1) {
		O_RotXAngle += 1.f;
	}
	else if (isORotateX == -1) {
		O_RotXAngle -= 1.f;
	}
	if (isLRotate == 1) {
		L_RotYAngle += 1.f;
	}
	else if (isLRotate == -1) {
		L_RotYAngle -= 1.f;
	}
	if (isLMove == 1) {
		L_MovValue += 0.01f;
	}
	else if (isLMove == -1) {
		if (L_MovValue + 4. > 2.)
			L_MovValue -= 0.01f;
	}

	CUBE.Update();
	PYRAMID.Update();
	LIGHT.Update();
	CIRCLE.Update();

	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}