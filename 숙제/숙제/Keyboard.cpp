#include "Keyboard.h"

int isCRevolution;
bool isUpDownMove = true;			//애니메이션 작동여부 //기본 true
int personView;

bool isW, isA, isS, isD;

bool isRobotDraw;

bool isMoveStopNDown;				//낮춰서 멈출건지	//기본 false

float MoveSpeed = 1.f;

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
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


	case 'm':
		isUpDownMove = true;
		break;
	case 'M':
		isUpDownMove = false;
		break;

	case 'r':						//미로 제작
	case 'R':
		newWorld.MakeMaze();
		break;

	case 'v':
		if (isMoveStopNDown) {
			isUpDownMove = true;
			isMoveStopNDown = false;
		}
		else {
			isUpDownMove = false;
			isMoveStopNDown = true;
		}
		break;
	case 'f':
		isRobotDraw = true;
		break;

	case '+':
		MoveSpeed += 0.25f;
		break;
	case '-':
		MoveSpeed -= 0.25f;
		if (MoveSpeed <= 0)
			MoveSpeed = 0;
		break;
		

	case 'o':
		personView = -1;
		break;

	case 'p':
	case '0':
		personView = 0;
		break;
	case '1':
		personView = 1;
		break;
	case '3':
		personView = 3;
		break;


	case 'z':
		C_movZ += 1.f;
		break;
	case 'Z':
		C_movZ -= 1.f;
		break;
	case 'x':
		C_movX += 1.f;
		break;
	case 'X':
		C_movX -= 1.f;
		break;
	case 'y':
		isCRevolution != 1 ? isCRevolution = 1 : isCRevolution = 0;
		break;
	case 'Y':
		isCRevolution != -1 ? isCRevolution = -1 : isCRevolution = 0;
		break;




	case 'c':							// 리셋
	case 'C':
		C_movX = 0.f;
		C_movZ = 0.f;
		C_RotYAngle = 0.f;
		isCRevolution = 0;
		isUpDownMove = true;			//애니메이션 작동여부 //기본 true
		personView = 0;
		isW = isA = isS = isD = false;
		MouseAngle = make_pair(0.f, 0.f);
		isRobotDraw = false;
		isMoveStopNDown = false;				//낮춰서 멈출건지	//기본 false
		MoveSpeed = 1.f;
		newWorld.KeyIn(key);
		break;
	case 'q':							// 종료
	case 'Q':
	case 27:
		glutLeaveMainLoop();
		break;
	case 'h':							// 은면제거
		isDepTest = !isDepTest;
		break;
	default:
		return;
	}
	cout << key << endl;

	//glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F5:
		glutFullScreenToggle();
		break;
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

	//glutPostRedisplay();
}

