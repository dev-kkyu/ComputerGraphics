#include "Keyboard.h"

bool isOpenF;

GLvoid Keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'o':
		isOpenF = true;
		break;
	case 'O':
		isOpenF = false;
		break;
	case 'w':
	case 'W':
	case 'a':
	case 'A':
	case 's':
	case 'S':
	case 'd':
	case 'D':
	case 'j':
	case 'J':
		robot.update(key);
		break;
	case 'z':
		break;
	case 'Z':
		break;
	case 'x':
		break;
	case 'X':
		break;
	case 'y':
		break;
	case 'Y':
		break;
	}

	glutPostRedisplay();
}