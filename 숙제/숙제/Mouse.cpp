#include "Mouse.h"

pair<float, float> MouseAngle;

GLvoid Mouse(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

	}

}

GLvoid Motion(int x, int y)
{

	//glutPostRedisplay();

}

GLvoid PassiveMotion(int x, int y)
{
	MouseAngle.first += ((x - winWidth / 2) / 12.5f);
	MouseAngle.second += ((y - winHeight / 2) / 12.5f);

	glutWarpPointer(winWidth / 2, winHeight / 2);

	//glutPostRedisplay();
}
