#include "Keyboard.h"

bool isOpenF;
bool isW, isA, isS, isD;
int isJump;

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
		/*if (isJump == 0) {
			isJump = 1;
		}
		else
			isJump = 2;*/
		if (isJump != 2)
		{
			++isJump;
		}

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


	case 'h':
		isDepTest = !isDepTest;
		break;
	}

	glutPostRedisplay();
}