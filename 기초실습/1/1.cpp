#include <iostream>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
bool TimerValue = false;

void main(int argc, char** argv)								//--- 윈도우 출력하고 콜백함수 설정 
{	//--- 윈도우 생성하기
	glutInit(&argc, argv);										// glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// 디스플레이 모드 설정
	glutInitWindowPosition(0, 0);							// 윈도우의 위치 지정
	glutInitWindowSize(800, 600);								// 윈도우의 크기 지정
	glutCreateWindow("Example1");								// 윈도우 생성(윈도우 이름)

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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
	//glutTimerFunc(100, TimerFunction, TimerValue);
	glutMainLoop();												// 이벤트 처리 시작
}

GLvoid drawScene()												//--- 콜백 함수: 그리기 콜백 함수
{
	//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);						// 바탕색을 ‘blue’ 로 지정
	glClear(GL_COLOR_BUFFER_BIT);								// 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.

	glutSwapBuffers();											// 화면에 출력하기
}

GLvoid Reshape(int w, int h)									//--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'R':
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		break; //--- 배경색을 빨강색으로 설정
	case 'G':
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		break; //--- 배경색을 초록색으로 설정
	case 'B':
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		break; //--- 배경색을 파랑색으로 설정
	case 'A': {
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<float> dis(0, 1);
		glClearColor(dis(gen), dis(gen), dis(gen), 1.0f);
		break; }
	case 'W':
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case 'K':
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 'T':
		TimerValue = true;
		glutTimerFunc(100, TimerFunction, TimerValue);
		break;
	case 'S':
		TimerValue = false;
		break;
	case 'Q':
		exit(0);
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid TimerFunction(int value)
{
	if (!value)
		return;

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0, 1);
	glClearColor(dis(gen), dis(gen), dis(gen), 1.0f);

	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(100, TimerFunction, TimerValue); // 타이머함수 재 설정
}