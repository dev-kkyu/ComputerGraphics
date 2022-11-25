#include "Timer.h"

int TimerValue = 10;

GLvoid TimerFunction(int value)
{
	if (isLRotate) {
		L_RotYAngle += 1.f * isLRotate;
	}
	if (isCRotate) {
		C_RotYAngle += 1.f * isCRotate;
	}
	LIGHT.Update();


	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}