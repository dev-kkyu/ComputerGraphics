#include "Timer.h"

int TimerValue = 10;

GLvoid TimerFunction(int value)
{
	if (isCRevolution == 1) {
		C_RotYAngle += 1.f;
	}
	else if (isCRevolution == -1) {
		C_RotYAngle -= 1.f;
	}

	for (Sphere*& sph : sphere) {
		sph->Update();
	}

	for (Cube& cub : cube) {
		cub.Update();
	}

	if (isFloorOpen) {
		for (Sphere*& sph : sphere) {
			sph->Drop();
		}

		for (Cube& cub : cube) {
			cub.Drop();
		}
	}

	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}