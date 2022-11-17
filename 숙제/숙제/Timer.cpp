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

	newWorld.Update();

	if (isRobotDraw) {
		if (isW) {
			newWorld.KeyIn('w');
		}
		if (isA) {
			newWorld.KeyIn('a');
		}
		if (isS) {
			newWorld.KeyIn('s');
		}
		if (isD) {
			newWorld.KeyIn('d');
		}
		if (isJump) {
			newWorld.KeyIn('j');
		}
	}

	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}