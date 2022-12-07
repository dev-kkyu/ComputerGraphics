#include "Keyboard.h"
#include "Light.h"
extern Light LIGHT;

int isLRotate;
bool isDrowSnow;

int isCRotate;

bool LightOff;

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		isLRotate != 1 ? isLRotate = 1 : isLRotate = 0;
		break;
	case 'R':
		isLRotate != -1 ? isLRotate = -1 : isLRotate = 0;
		break;
	case 'y':
		isCRotate != 1 ? isCRotate = 1 : isCRotate = 0;
		break;
	case 'Y':
		isCRotate != -1 ? isCRotate = -1 : isCRotate = 0;
		break;
	case 'm':
	case 'M':
		LightOff = not LightOff;
		if (LightOff) {
			GLuint LightOffLocation =  glGetUniformLocation(shaderID, "LightOff");
			glUniform1i(LightOffLocation, 1);
		}
		else {
			GLuint LightOffLocation = glGetUniformLocation(shaderID, "LightOff");
			glUniform1i(LightOffLocation, 0);
		}
		break;
	case 's':
	case 'S':
		isDrowSnow = not isDrowSnow;
		break;
	case '0':
		FractalNum = 0;
		break;
	case '1':
		FractalNum = 1;
		break;
	case '2':
		FractalNum = 2;
		break;
	case '3':
		FractalNum = 3;
		break;
	case '4':
		FractalNum = 4;
		break;
	case '5':
		FractalNum = 5;
		break;
	case '6':
		FractalNum = 6;
		break;
	case '7':
		FractalNum = 7;
		break;
	case '8':
		FractalNum = 8;
		break;
	case 'i':
	case 'o':
	case '+':
	case '-':
		LIGHT.EditValue(key);
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

