#include "Timer.h"

int TimerValue = 10;

GLvoid TimerFunction(int value)
{
	if (isLRotate == 1) {
		L_RotYAngle += 1.f;
	}
	else if (isLRotate == -1) {
		L_RotYAngle -= 1.f;
	}

	LIGHT.Update();
	for (Sphere& sph : SPHERE)
		sph.Update();

	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}