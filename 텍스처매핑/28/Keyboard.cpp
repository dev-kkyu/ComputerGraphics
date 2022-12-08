#include "Keyboard.h"

int isORotateY;
int isORotateX;
int isLRotate;

int isLMove;

bool isLight;

bool isRect = true;

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'y':
		isORotateY != 1 ? isORotateY = 1 : isORotateY = 0;
		break;
	case 'Y':
		isORotateY != -1 ? isORotateY = -1 : isORotateY = 0;
		break;
	case 'x':
		isORotateX != 1 ? isORotateX = 1 : isORotateX = 0;
		break;
	case 'X':
		isORotateX != -1 ? isORotateX = -1 : isORotateX = 0;
		break;
	case 'r':
		isLRotate != 1 ? isLRotate = 1 : isLRotate = 0;
		break;
	case 'R':
		isLRotate != -1 ? isLRotate = -1 : isLRotate = 0;
		break;
	case 'c':
		isRect = true;
		break;
	case 'p':
		isRect = false;
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
	case 's':
		O_RotYAngle = 0.f;
		isORotateY = 0;
		O_RotXAngle = 0.f;
		isORotateX = 0;

		L_RotYAngle = 0.f;
		isLRotate = 0;

		L_MovValue = 0.f;
		isLMove = 0;

		isRect = true;
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

