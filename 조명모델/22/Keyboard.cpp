#include "Keyboard.h"

int isORotate;
int isLRotate;

int isLMove;

bool isLight;

bool isRect = true;

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'y':
		isORotate != 1 ? isORotate = 1 : isORotate = 0;
		break;
	case 'Y':
		isORotate != -1 ? isORotate = -1 : isORotate = 0;
		break;
	case 'r':
		isLRotate != 1 ? isLRotate = 1 : isLRotate = 0;
		break;
	case 'R':
		isLRotate != -1 ? isLRotate = -1 : isLRotate = 0;
		break;
	case 'n':
		isRect = not isRect;
		break;
	case 'm':
		if (isLight) {
			GLuint lightOption = glGetUniformLocation(shaderID, "option");
			glUniform1i(lightOption, 0);
		}
		else {
			GLuint lightOption = glGetUniformLocation(shaderID, "option");
			glUniform1i(lightOption, 1);
		}
		isLight = not isLight;
		break;
	case 'z':
		isLMove != 1 ? isLMove = 1 : isLMove = 0;
		break;
	case 'Z':
		isLMove != -1 ? isLMove = -1 : isLMove = 0;
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

