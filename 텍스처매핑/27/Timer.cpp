#include "Timer.h"
#include "Light.h"
extern Light LIGHT;
#include "Snow.h"
extern Snow SNOW[500];
#include "Cube.h"
extern Cube CUBE[12];

int TimerValue = 10;

GLvoid TimerFunction(int value)
{
	if (isLRotate == 1) {
		L_RotYAngle += 1.f;
	}
	else if (isLRotate == -1) {
		L_RotYAngle -= 1.f;
	}
	if (isCRotate == 1) {
		cameraRotAngle += 1.f;
	}
	else if (isCRotate == -1) {
		cameraRotAngle -= 1.f;
	}

	LIGHT.Update();

	for (Cube& cube : CUBE)
		cube.Update();

	for (Snow& snow : SNOW) {
		snow.Update();
	}

	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}