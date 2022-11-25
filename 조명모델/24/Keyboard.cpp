#include "Keyboard.h"

int colorValue;
int isLRotate;
int isCRotate;

bool isLightOff;

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'm':
		isLightOff = not isLightOff;
		if (isLightOff) {
			GLuint LightOff = glGetUniformLocation(shaderID, "LightOff");
			glUniform1i(LightOff, 1);
		}
		else {
			GLuint LightOff = glGetUniformLocation(shaderID, "LightOff");
			glUniform1i(LightOff, 0);
		}
		break;
	case 'r':
		isLRotate != 1 ? isLRotate = 1 : isLRotate = 0;
		break;
	case 'R':
		isLRotate != -1 ? isLRotate = -1 : isLRotate = 0;
		break;
	case 's':
	case 'S':
		isLRotate = 0;
		break;
	case 'c':
	case 'C':
		colorValue = (colorValue + 1) % 4;
		break;
	case 'z':
		C_movZ += 0.1f;
		break;
	case 'Z':
		C_movZ -= 0.1f;
		break;
	case 'x':
		C_movX += 0.1f;
		break;
	case 'X':
		C_movX -= 0.1f;
		break;
	case 'y':
		isCRotate != 1 ? isCRotate = 1 : isCRotate = 0;
		break;
	case 'Y':
		isCRotate != -1 ? isCRotate = -1 : isCRotate = 0;
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

