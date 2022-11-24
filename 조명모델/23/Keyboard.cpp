#include "Keyboard.h"

int isLRotate;
int colorValue;

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		isLRotate != 1 ? isLRotate = 1 : isLRotate = 0;
		break;
	case 'R':
		isLRotate != -1 ? isLRotate = -1 : isLRotate = 0;
		break;
	case 'c':
	case 'C':
		colorValue = (colorValue + 1) % 4;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}

	//glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}

	//glutPostRedisplay();
}

