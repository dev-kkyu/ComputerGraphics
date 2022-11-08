#include "Keyboard.h"

bool isOpenF;
bool isW, isA, isS, isD;
int isJump;

int isCRevolution;

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
		isW = true;
		break;
	case 'a':
	case 'A':
		isA = true;
		break;
	case 's':
	case 'S':
		isS = true;
		break;
	case 'd':
	case 'D':
		isD = true;
		break;
	case 'j':
	case 'J':
	case ' ':
		if (isJump != 2)
		{
			++isJump;
		}
		break;
	case 'i':
	case 'I':
		robot.update('i');
		break;
	case 'z':
		C_movZ += 0.05;
		break;
	case 'Z':
		C_movZ -= 0.05;
		break;
	case 'x':
		C_movX += 0.05;
		break;
	case 'X':
		C_movX -= 0.05;
		break;
	case 'y':
		isCRevolution != 1 ? isCRevolution = 1 : isCRevolution = 0;
		break;
	case 'Y':
		isCRevolution != -1 ? isCRevolution = -1 : isCRevolution = 0;
		break;

	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;
	case 'h':
		isDepTest = !isDepTest;
		break;
	}

	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		isA = true;
		break;
	case GLUT_KEY_RIGHT:
		isD = true;
		break;
	case GLUT_KEY_UP:
		isW = true;
		break;
	case GLUT_KEY_DOWN:
		isS = true;
		break;
	}

	glutPostRedisplay();
}