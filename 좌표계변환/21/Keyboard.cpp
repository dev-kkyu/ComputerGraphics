#include "Keyboard.h"

int isCRevolution;
bool isFloorOpen;

float sphereScale = 0.06f;

GLvoid Keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'b':
	case 'B':
		if (sphere.size() < 5) {
			sphere.push_back(new Sphere(sphereScale));
			sphereScale += 0.02f;
		}
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


	case 'r':
	case 'R':
		C_movX = 0.f;
		C_movZ = 0.f;
		isCRevolution = 0;
		C_RotYAngle = 0.f;
		break;
	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;
	case 'h':
		isDepTest = !isDepTest;
		break;
	}

	//glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F5:
		isFloorOpen = true;
		break;
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