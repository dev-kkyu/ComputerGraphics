#include "Timer.h"
#include "Light.h"
extern Light LIGHT;
#include "Sphere.h"
extern Sphere SPHERE[3];
#include "Snow.h"
extern Snow SNOW[100];

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

	for (Snow& snow : SNOW) {
		snow.Update();
	}

	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}