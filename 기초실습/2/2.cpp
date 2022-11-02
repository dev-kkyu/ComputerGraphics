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

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);

GLXY Win2GL(int x, int y);

int winWidth = 500;					// 현재 창 가로
int winHeight = 500;				// 현재 창 세로

GLXY_Pair Rect = { {-0.5f, -0.5f}, {0.5f, 0.5f} };		// 사각형 한개의 좌표들

void main(int argc, char** argv)								//--- 윈도우 출력하고 콜백함수 설정 
{	//--- 윈도우 생성하기
	glutInit(&argc, argv);										// glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// 디스플레이 모드 설정
	glutInitWindowPosition(0, 0);							// 윈도우의 위치 지정
	glutInitWindowSize(winWidth, winHeight);								// 윈도우의 크기 지정
	glutCreateWindow("Example1");								// 윈도우 생성(윈도우 이름)

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);				// 초기 배경색 : 화이트
	glColor3f(1.0f, 0.0f, 0.0f);						// 초기 사각형 색 : 레드


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
	glutMouseFunc(Mouse);
	glutMainLoop();												// 이벤트 처리 시작
}

GLvoid drawScene()												//--- 콜백 함수: 그리기 콜백 함수
{
	//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);						// 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT);								// 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.

	glRectf(Rect.r1.x, Rect.r1.y, Rect.r2.x, Rect.r2.y);

	glutSwapBuffers();											// 화면에 출력하기
}

GLvoid Reshape(int w, int h)									//--- 콜백 함수: 다시 그리기 콜백 함수
{
	winWidth = w;
	winHeight = h;

	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	GLXY data1 = Win2GL(x, y);
	GLXY data2 = Win2GL(x, y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<float> dis(0, 1);
		if (data1.x > Rect.r1.x && data1.y > Rect.r1.y && data2.x < Rect.r2.x && data2.y < Rect.r2.y)
			glColor3f(dis(gen), dis(gen), dis(gen));
		else
			glClearColor(dis(gen), dis(gen), dis(gen), 1.0f);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (data1.x > Rect.r1.x && data1.y > Rect.r1.y && data2.x < Rect.r2.x && data2.y < Rect.r2.y) {
			Rect.r1.x -= 0.05f;
			Rect.r1.y -= 0.05f;
			Rect.r2.x += 0.05f;
			Rect.r2.y += 0.05f;
			if (Rect.r1.x < -0.9f) {
				Rect.r1.x = -0.9f;
				Rect.r1.y = -0.9f;
				Rect.r2.x = 0.9f;
				Rect.r2.y = 0.9f;
			}
		}
		else {
			Rect.r1.x += 0.05f;
			Rect.r1.y += 0.05f;
			Rect.r2.x -= 0.05f;
			Rect.r2.y -= 0.05f;
			if (Rect.r1.x > -0.1f) {
				Rect.r1.x = -0.1f;
				Rect.r1.y = -0.1f;
				Rect.r2.x = 0.1f;
				Rect.r2.y = 0.1f;
			}
		}
	}

}

GLXY Win2GL(int x, int y)
{
	GLXY data;
	data.x = (float)(x - (float)winWidth / 2.0) * (float)(1.0 / (float)(winWidth / 2.0));
	data.y = -(float)(y - (float)winHeight / 2.0) * (float)(1.0 / (float)(winHeight / 2.0));

	return data;
}
