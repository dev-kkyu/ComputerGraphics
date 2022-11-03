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


	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}