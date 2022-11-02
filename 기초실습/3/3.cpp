#include <iostream>
#include <random>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

using namespace std;

struct GLXY {
	float x;
	float y;
};

struct GLXY_Pair {
	GLXY r1;
	GLXY r2;
};

struct WinXY {
	int x;
	int y;
};

class Rect {
public:
	WinXY rect;
	float color[3];

	static int seq;

	Rect();
	~Rect();
};
int Rect::seq = 0;


// 콜백함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

// 좌표변환함수
GLXY Win2GL(int x, int y);
WinXY GL2Win(float x, float y);


// 전역변수
int winWidth = 600;					// 현재 창 가로
int winHeight = 600;				// 현재 창 세로

Rect* rect[5]{};

bool MotionAble = false;
int MotionIndex;

void main(int argc, char** argv)								//--- 윈도우 출력하고 콜백함수 설정 
{	//--- 윈도우 생성하기
	glutInit(&argc, argv);										// glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// 디스플레이 모드 설정
	glutInitWindowPosition(0, 0);							// 윈도우의 위치 지정
	glutInitWindowSize(winWidth, winHeight);								// 윈도우의 크기 지정
	glutCreateWindow("Example1");								// 윈도우 생성(윈도우 이름)

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);				// 초기 배경색 : 화이트

	rect[Rect::seq - 1] = new Rect;

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)									// glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);									// 출력 함수의 지정
	glutReshapeFunc(Reshape);									// 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);									// 키보드 입력 콜백함수 지정
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();												// 이벤트 처리 시작
}

GLvoid drawScene()												//--- 콜백 함수: 그리기 콜백 함수
{
	//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);						// 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT);								// 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.

	for (int i = 0; i < Rect::seq; ++i) {
		GLXY_Pair data = { Win2GL(rect[i]->rect.x - 40, rect[i]->rect.y + 40),Win2GL(rect[i]->rect.x + 40, rect[i]->rect.y - 40)};
		glColor3f(rect[i]->color[0], rect[i]->color[1], rect[i]->color[2]);						// 초기 사각형 색 : 레드
		glRectf(data.r1.x, data.r1.y, data.r2.x, data.r2.y);
	}

	glutSwapBuffers();											// 화면에 출력하기
}

GLvoid Reshape(int w, int h)									//--- 콜백 함수: 다시 그리기 콜백 함수
{
	winWidth = w;
	winHeight = h;

	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		if(Rect::seq<5)
			rect[Rect::seq - 1] = new Rect;
		break;
	default:
		break;
	}

	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		for (int i = Rect::seq - 1; i >= 0; --i) {
			if (rect[i]->rect.x - 40 < x && rect[i]->rect.x + 40 > x && rect[i]->rect.y - 40 < y && rect[i]->rect.y + 40 > y) {
				MotionAble = true;
				MotionIndex = i;
				break;
			}
		}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		MotionAble = false;
}

GLvoid Motion(int x, int y)
{
	//for (int i = Rect::seq - 1; i >= 0; --i) {
	//	if (rect[i]->rect.x - 40 < x && rect[i]->rect.x + 40 > x && rect[i]->rect.y - 40 < y && rect[i]->rect.y + 40 > y) {
	//		rect[i]->rect.x = x;
	//		rect[i]->rect.y = y;
	//		break;
	//	}
	//}
	//glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
	if (MotionAble) {
		rect[MotionIndex]->rect.x = x;
		rect[MotionIndex]->rect.y = y;
		glutPostRedisplay();
	}
}

GLXY Win2GL(int x, int y)
{
	GLXY data;
	data.x = (float)(x - (float)winWidth / 2.0) * (float)(1.0 / (float)(winWidth / 2.0));
	data.y = -(float)(y - (float)winHeight / 2.0) * (float)(1.0 / (float)(winHeight / 2.0));

	return data;
}

WinXY GL2Win(float x, float y)
{
	WinXY data;
	data.x = (1. + x) * (winWidth / 2);
	data.y = -(-1. + y) * (winHeight / 2);

	return data;
}

Rect::Rect()
{
	rect.x = winWidth / 2;
	rect.y = winHeight / 2;

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.f, 1.f);
	for (int i = 0; i < 3; ++i)
		color[i] = dis(gen);

	++seq;
}

Rect::~Rect()
{
	--seq;
}