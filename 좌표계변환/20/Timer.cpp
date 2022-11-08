#include "Timer.h"

int TimerValue = 10;

GLvoid TimerFunction(int value)
{
	if (isOpenF) {
		if (openF <= 1.99f)
			openF += 0.01f;
	}
	else {
		if (openF >= 0.01)
			openF -= 0.01f;
	}
	if (isW) {
		robot.update('w');
	}
	if (isA) {
		robot.update('a');
	}
	if (isS) {
		robot.update('s');
	}
	if (isD) {
		robot.update('d');
	}
	if (isJump) {
		robot.update('j');
	}


	if (isCRevolution == 1) {
		C_RotYAngle += 1.f;
	}
	else if (isCRevolution == -1) {
		C_RotYAngle -= 1.f;
	}


	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}