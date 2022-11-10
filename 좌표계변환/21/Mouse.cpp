#include "Mouse.h"

pair<int, int> startMousePT;
float endAngle;

GLvoid Mouse(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		startMousePT = make_pair(x, y);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		endAngle = CubeAngle;
}

GLvoid Motion(int x, int y)
{
	if (y <= winHeight / 2)
		CubeAngle = endAngle - ((x - startMousePT.first) / 5.);
	else
		CubeAngle = endAngle + ((x - startMousePT.first) / 5.);


	//glutPostRedisplay();

}

GLvoid PassiveMotion(int x, int y)
{
	
	//glutPostRedisplay();
}
