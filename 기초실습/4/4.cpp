//#include <iostream>
//#include <random>
//#include <gl/glew.h>											//--- 필요한 헤더파일 include
//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//
//using namespace std;
//
//struct GLXY {
//	float x;
//	float y;
//};
//
//struct GLXY_Pair {
//	GLXY r1;
//	GLXY r2;
//};
//
//class WinXY {
//public:
//	int x;
//	int y;
//
//	WinXY() { x = 0; y = 0; }
//	WinXY(int xx, int yy) { x = xx; y = yy; }
//};
//
//class Rect {
//private:
//	WinXY start;
//public:
//	WinXY direction;
//	WinXY rect;
//	float color[3];
//	WinXY rectSize;
//
//	short moveCount;
//
//	static int seq;
//
//	void resetDirection();
//	void init();
//	Rect();
//	Rect(WinXY data);
//	~Rect();
//	void move();
//
//	void reset() { rect = start; }
//};
//int Rect::seq = 0;
//
//
//// 콜백함수
//GLvoid drawScene(GLvoid);
//GLvoid Reshape(int w, int h);
//GLvoid Keyboard(unsigned char key, int x, int y);
//GLvoid Mouse(int button, int state, int x, int y);
//GLvoid TimerFunction(int value);
//
//// 좌표변환함수
//GLXY Win2GL(int x, int y);
//WinXY GL2Win(float x, float y);
//
//bool isCollide(Rect* r1, Rect* r2);
//
//// 전역변수
//int winWidth = 600;					// 현재 창 가로
//int winHeight = 600;				// 현재 창 세로
//
//short timee = 3;
//unsigned char TimerValue = 0;
//bool isAni = false;
//char lastAni = 'a';
//short c_Count = 0;
//bool isC = false;
////bool isStop = false;
//
//Rect* rect[5]{};
//
//void main(int argc, char** argv)								//--- 윈도우 출력하고 콜백함수 설정 
//{	//--- 윈도우 생성하기
//	glutInit(&argc, argv);										// glut 초기화
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// 디스플레이 모드 설정
//	glutInitWindowPosition(0, 0);							// 윈도우의 위치 지정
//	glutInitWindowSize(winWidth, winHeight);								// 윈도우의 크기 지정
//	glutCreateWindow("Example1");								// 윈도우 생성(윈도우 이름)
//
//	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);				// 초기 배경색 : 짙은 회색
//
//	//--- GLEW 초기화하기
//	glewExperimental = GL_TRUE;
//	if (glewInit() != GLEW_OK)									// glew 초기화
//	{
//		std::cerr << "Unable to initialize GLEW" << std::endl;
//		exit(EXIT_FAILURE);
//	}
//	else
//		std::cout << "GLEW Initialized\n";
//
//	glutDisplayFunc(drawScene);									// 출력 함수의 지정
//	glutReshapeFunc(Reshape);									// 다시 그리기 함수 지정
//	glutKeyboardFunc(Keyboard);									// 키보드 입력 콜백함수 지정
//	glutMouseFunc(Mouse);
//	glutMainLoop();												// 이벤트 처리 시작
//}
//
//GLvoid drawScene()												//--- 콜백 함수: 그리기 콜백 함수
//{
//	glClear(GL_COLOR_BUFFER_BIT);								// 설정된 색으로 전체를 칠하기
//	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
//
//	for (int i = 0; i < Rect::seq; ++i) {
//		GLXY_Pair data = { Win2GL(rect[i]->rect.x - rect[i]->rectSize.x / 2, rect[i]->rect.y + rect[i]->rectSize.y / 2),Win2GL(rect[i]->rect.x + rect[i]->rectSize.x / 2, rect[i]->rect.y - rect[i]->rectSize.y / 2) };
//		glColor3f(rect[i]->color[0], rect[i]->color[1], rect[i]->color[2]);						// 초기 사각형 색 : 레드
//		glRectf(data.r1.x, data.r1.y, data.r2.x, data.r2.y);
//	}
//
//	glutSwapBuffers();											// 화면에 출력하기
//}
//
//GLvoid Reshape(int w, int h)									//--- 콜백 함수: 다시 그리기 콜백 함수
//{
//	winWidth = w;
//	winHeight = h;
//
//	glViewport(0, 0, w, h);
//}
//
//GLvoid Keyboard(unsigned char key, int x, int y)
//{
//	cout << key << endl;
//
//	random_device rd;
//	mt19937 gen(rd());
//	uniform_int_distribution<int> dis(0, 1);
//	switch (key) {
//	case 'a':
//		if (isAni) {
//			TimerValue = 0;
//			isAni = false;
//			return;
//		}
//		if (lastAni != 'a')
//			for (int i = 0; i < Rect::seq; ++i) {
//			rect[i]->resetDirection();
//			}
//			break;
//	case 'i': 								// 지그재그 방향 정해주기
//		if (isAni) {
//			TimerValue = 0;
//			isAni = false;
//			return;
//		}
//		if (lastAni != 'i')
//			for (int i = 0; i < Rect::seq; ++i) {
//				rect[i]->resetDirection();
//				if (dis(gen))
//					rect[i]->direction.x *= 2;
//				else
//					rect[i]->direction.y *= 2;
//			}
//		break;
//	case 'c':
//		isC = !isC;
//		if (!isAni) {
//			if (isC) {
//				TimerValue = key;
//				glutTimerFunc(timee, TimerFunction, TimerValue);
//			}
//			else
//				TimerValue = 0;
//		}
//		return;
//	case 's':
//		isAni = false;
//		TimerValue = 0;
//		return;
//	case 'm':
//		for (int i = 0; i < Rect::seq; ++i)
//			rect[i]->reset();
//		glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
//		return;
//	case 'r':
//		for (int i = Rect::seq - 1; i >= 0; --i)
//			delete rect[i];
//		c_Count = 0;
//		glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
//		return;
//	case 'q':
//		//exit(0);
//		glutLeaveMainLoop();
//	default:
//		cout << "설정된 이벤트 없음" << endl;
//		return;
//		
//	}
//
//	// a, i에 공통적으로 수행
//	lastAni = key;					// 마지막에 했던 애니메이션 저장
//	TimerValue = key;
//	/*if (isAni) {
//		TimerValue = 0;
//		isAni = false;
//	}*/
//	//else {
//		isAni = true;
//		glutTimerFunc(timee, TimerFunction, TimerValue);
//		//if (isC) {												// a, i를 실행하려고 할 때 isC가 이미 실행중이면
//		//	isStop = true;										// 일단 한번 멈추고 실행하자
//		//}
//	//}
//
//}
//
//GLvoid Mouse(int button, int state, int x, int y)				// 여기 xy는 윈도우좌표계
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//		if (Rect::seq < 5)
//			rect[Rect::seq - 1] = new Rect(WinXY(x, y));
//	}
//}
//
//GLvoid TimerFunction(int value)
//{
//	/*if (isStop) {
//		isStop = false;
//		return;
//	}*/
//
//	switch (value) {
//	case 0:
//		return;
//	case 'a':
//		for (int i = 0; i < Rect::seq; ++i) {
//			if (rect[i]->rect.x - rect[i]->rectSize.x / 2 <= 0 || rect[i]->rect.x + rect[i]->rectSize.x / 2 >= winWidth)				// 왼쪽이나 오른쪽에 박으면
//				rect[i]->direction.x *= -1;													// x좌표 반대로
//			else if (rect[i]->rect.y - rect[i]->rectSize.y / 2 <= 0 || rect[i]->rect.y + rect[i]->rectSize.y / 2 >= winHeight)		// 위나 아래에 박으면
//				rect[i]->direction.y *= -1;													// y좌표 반대로
//
//			for (int j = i; j < Rect::seq; ++j) {		// 사각형끼리 충돌하면
//				if (isCollide(rect[i], rect[j])) {		// 방향 reverse해주도록 함
//					rect[i]->direction.x *= -1;
//					rect[i]->direction.y *= -1; 
//					rect[j]->direction.x *= -1;
//					rect[j]->direction.y *= -1;
//				}
//			}
//
//			rect[i]->move();																// 이동
//
//			//while (true) {
//			//	switch (rect[i]->isDirection()) {			// 방향에 따라
//			//	case 0:										// 왼위로 갈때
//			//		if (rect[i]->rect.y - 40 <= 0) {		// 위에 박으면
//			//			rect[i]->setDirection(3);			// 왼아래로
//			//			continue;
//			//		}
//			//		if (rect[i]->rect.x - 40 <= 0) {
//			//			rect[i]->setDirection(1);
//			//			continue;
//			//		}
//			//		rect[i]->add(WinXY(-1, -1));
//			//		break;
//			//	case 1:
//			//		if (rect[i]->rect.y - 40 <= 0) {
//			//			rect[i]->setDirection(2);
//			//			continue;
//			//		}
//			//		if (rect[i]->rect.x + 40 >= winWidth) {
//			//			rect[i]->setDirection(0);
//			//			continue;
//			//		}
//			//		rect[i]->add(WinXY(1, -1));
//			//		break;
//			//	case 2:
//			//		if (rect[i]->rect.y + 40 >= winHeight) {
//			//			rect[i]->setDirection(1);
//			//			continue;
//			//		}
//			//		if (rect[i]->rect.x + 40 >= winWidth) {
//			//			rect[i]->setDirection(3);
//			//			continue;
//			//		}
//			//		rect[i]->add(WinXY(1, 1));
//			//		break;
//			//	case 3:
//			//		if (rect[i]->rect.y + 40 >= winHeight) {
//			//			rect[i]->setDirection(0);
//			//			continue;
//			//		}
//			//		if (rect[i]->rect.x - 40 <= 0) {
//			//			rect[i]->setDirection(2);
//			//			continue;
//			//		}
//			//		rect[i]->add(WinXY(-1, 1));
//			//		break;
//			//	}
//			//	break;
//			//}
//		}
//		break;
//	case 'i': 
//		for (int i = 0; i < Rect::seq; ++i) {
//			if (rect[i]->rect.x - rect[i]->rectSize.x / 2 <= 0 || rect[i]->rect.x + rect[i]->rectSize.x / 2 >= winWidth) {				// 왼쪽이나 오른쪽에 박으면
//				rect[i]->direction.x *= -1;													// x좌표 반대로
//				rect[i]->moveCount = 0;														// 카운트 초기화
//			}
//			else if (rect[i]->rect.y - rect[i]->rectSize.y / 2 <= 0 || rect[i]->rect.y + rect[i]->rectSize.y / 2 >= winHeight) {		// 위나 아래에 박으면
//				rect[i]->direction.y *= -1;													// y좌표 반대로
//				rect[i]->moveCount = 0;
//			}
//
//			if (rect[i]->moveCount == 100) {												//100번 움직이면
//				rect[i]->moveCount = 0;														//초기화
//				if ((rect[i]->direction.x < 0 ? -rect[i]->direction.x : rect[i]->direction.x) == 2)		//direction이 2인곳에다가 방향바꿔줌
//					rect[i]->direction.x *= -1;
//				else if ((rect[i]->direction.y < 0 ? -rect[i]->direction.y : rect[i]->direction.y) == 2)
//					rect[i]->direction.y *= -1;
//			}
//
//			rect[i]->move();																// 이동
//			++rect[i]->moveCount;
//		}
//		break;
//	}
//	
//	if (isC) {
//		for (int i = 0; i < Rect::seq; ++i) {
//			switch (c_Count / 80) {
//			case 0:
//				++rect[i]->rectSize.x;
//				rect[i]->rectSize.y += 2;
//				break;
//			case 1:
//				--rect[i]->rectSize.x;
//				rect[i]->rectSize.y -= 2;
//				break;
//			case 2:
//				++rect[i]->rectSize.y;
//				rect[i]->rectSize.x += 2;
//				break;
//			case 3:
//				--rect[i]->rectSize.y;
//				rect[i]->rectSize.x -= 2;
//				break;
//			default:
//				c_Count = 0;
//				break;
//			}
//		}
//		++c_Count;
//	}
//
//	glutPostRedisplay(); // 화면 재 출력
//	glutTimerFunc(timee, TimerFunction, TimerValue); // 타이머함수 재 설정
//}
//
//GLXY Win2GL(int x, int y)
//{
//	GLXY data;
//	data.x = (float)(x - (float)winWidth / 2.0) * (float)(1.0 / (float)(winWidth / 2.0));
//	data.y = -(float)(y - (float)winHeight / 2.0) * (float)(1.0 / (float)(winHeight / 2.0));
//
//	return data;
//}
//
//WinXY GL2Win(float x, float y)
//{
//	WinXY data;
//	data.x = (int)((1.f + x) * ((float)winWidth / 2.f));
//	data.y = -(int)((-1.f + y) * ((float)winHeight / 2.f));
//
//	return data;
//}
//
//void Rect::resetDirection()
//{
//	random_device rd;
//	mt19937 gen(rd());
//	uniform_int_distribution<int> dis2(0, 3);						// 최초 방향 설정
//	switch (dis2(gen)) {
//	case 0:
//		direction = WinXY(-1, -1);
//		break;
//	case 1:
//		direction = WinXY(1, -1);
//		break;
//	case 2:
//		direction = WinXY(1, 1);
//		break;
//	case 3:
//		direction = WinXY(-1, 1);
//		break;
//	}
//}
//
//void Rect::init()
//{
//	start = rect;													// 최초 좌표 기억
//
//	rectSize = WinXY(80, 80);										// 사각형 사이즈 지정
//	moveCount = 0;
//
//	random_device rd;
//	mt19937 gen(rd());
//	uniform_real_distribution<float> dis(0.f, 1.f);					// 색상 설정
//	for (int i = 0; i < 3; ++i)
//		color[i] = dis(gen);
//
//	resetDirection();													// 최초 방향 설정
//
//	++seq;
//}
//
//Rect::Rect()
//{
//	rect = WinXY(winWidth / 2, winHeight / 2);
//
//	init();
//}
//
//Rect::Rect(WinXY data)
//{
//	rect = data;
//
//	init();
//}
//
//Rect::~Rect()
//{
//	--seq;
//}
//
//void Rect::move()
//{
//	rect.x += direction.x;
//	rect.y += direction.y;
//}
//
//
//bool isCollide(Rect* r1, Rect* r2)
//{
//	if (r1->rect.x + r1->rectSize.x / 2 < r2->rect.x - r2->rectSize.x / 2 || r1->rect.x - r1->rectSize.x / 2 > r2->rect.x + r2->rectSize.x / 2) return false;
//	if (r1->rect.y + r1->rectSize.y / 2 < r2->rect.y - r2->rectSize.y / 2 || r1->rect.y - r1->rectSize.y / 2 > r2->rect.y + r2->rectSize.y / 2) return false;
//	return true;
//}








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

class WinXY {
public:
	int x;
	int y;

	WinXY() { x = 0; y = 0; }
	WinXY(int xx, int yy) { x = xx; y = yy; }
};

class Rect {
private:
	WinXY start;
public:
	WinXY direction;
	WinXY rect;
	float color[3];
	WinXY rectSize;

	short moveCount;

	static int seq;

	void resetDirection();
	void init();
	Rect();
	Rect(WinXY data);
	~Rect();
	void move();

	void reset() { rect = start; }
};
int Rect::seq = 0;


// 콜백함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid TimerFunction(int value);

// 좌표변환함수
GLXY Win2GL(int x, int y);
WinXY GL2Win(float x, float y);

bool isCollide(Rect* r1, Rect* r2);

// 전역변수
int winWidth = 600;					// 현재 창 가로
int winHeight = 600;				// 현재 창 세로

short timee = 2;
//unsigned char TimerValue = 0;
bool isAni = false;
char lastAni = 'a';
short c_Count = 0;
bool isC = false;
//bool isStop = false;

Rect* rect[10]{};

void main(int argc, char** argv)								//--- 윈도우 출력하고 콜백함수 설정 
{	//--- 윈도우 생성하기
	cout << sizeof(Rect) << endl;
	glutInit(&argc, argv);										// glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// 디스플레이 모드 설정
	glutInitWindowPosition(0, 0);							// 윈도우의 위치 지정
	glutInitWindowSize(winWidth, winHeight);								// 윈도우의 크기 지정
	glutCreateWindow("Example1");								// 윈도우 생성(윈도우 이름)

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);				// 초기 배경색 : 짙은 회색

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
	glutMainLoop();												// 이벤트 처리 시작
}

GLvoid drawScene()												//--- 콜백 함수: 그리기 콜백 함수
{
	glClear(GL_COLOR_BUFFER_BIT);								// 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.

	for (int i = 0; i < Rect::seq; ++i) {
		GLXY_Pair data = { Win2GL(rect[i]->rect.x - rect[i]->rectSize.x / 2, rect[i]->rect.y + rect[i]->rectSize.y / 2),Win2GL(rect[i]->rect.x + rect[i]->rectSize.x / 2, rect[i]->rect.y - rect[i]->rectSize.y / 2) };
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
	cout << key << endl;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 1);
	switch (key) {
	case 'a':
		if (isAni) {
			//TimerValue = 0;
			isAni = false;
			return;
		}
		if (lastAni != 'a')
			for (int i = 0; i < Rect::seq; ++i) {
				rect[i]->resetDirection();
			}
		break;
	case 'i': 								// 지그재그 방향 정해주기
		if (isAni) {
			//TimerValue = 0;
			isAni = false;
			return;
		}
		if (lastAni != 'i')
			for (int i = 0; i < Rect::seq; ++i) {
				rect[i]->resetDirection();
				if (dis(gen))
					rect[i]->direction.x *= 2;
				else
					rect[i]->direction.y *= 2;
			}
		break;
	case 'c':
		isC = !isC;
		/*if (!isAni) {
			if (isC) {
				TimerValue = key;
				glutTimerFunc(timee, TimerFunction, TimerValue);
			}
			else
				TimerValue = 0;
		}*/
		//return;
		break;
	case 's':
		//isAni = false;
		//TimerValue = 0;
		return;
	case 'm':
		for (int i = 0; i < Rect::seq; ++i)
			rect[i]->reset();
		glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
		return;
	case 'r':
		for (int i = Rect::seq - 1; i >= 0; --i)
			delete rect[i];
		//c_Count = 0;
		glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
		return;
	case 'q':
		//exit(0);
		glutLeaveMainLoop();
	default:
		cout << "설정된 이벤트 없음" << endl;
		return;

	}

	// a, i에 공통적으로 수행
	lastAni = key;					// 마지막에 했던 애니메이션 저장
	//TimerValue = key;
	/*if (isAni) {
		TimerValue = 0;
		isAni = false;
	}*/
	//else {
	isAni = true;
	//glutTimerFunc(timee, TimerFunction, TimerValue);
	glutTimerFunc(timee, TimerFunction, key);
	//if (isC) {												// a, i를 실행하려고 할 때 isC가 이미 실행중이면
	//	isStop = true;										// 일단 한번 멈추고 실행하자
	//}
//}

}

GLvoid Mouse(int button, int state, int x, int y)				// 여기 xy는 윈도우좌표계
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (Rect::seq < 10)
			rect[Rect::seq - 1] = new Rect(WinXY(x, y));
	}
}

GLvoid TimerFunction(int value)
{
	if (!isAni) {
		return;
	}

	switch (value) {
	case 0:
		return;
	case 'a':
		for (int i = 0; i < Rect::seq; ++i) {
			if (rect[i]->rect.x - rect[i]->rectSize.x / 2 <= 0 || rect[i]->rect.x + rect[i]->rectSize.x / 2 >= winWidth)				// 왼쪽이나 오른쪽에 박으면
				rect[i]->direction.x *= -1;													// x좌표 반대로
			else if (rect[i]->rect.y - rect[i]->rectSize.y / 2 <= 0 || rect[i]->rect.y + rect[i]->rectSize.y / 2 >= winHeight)		// 위나 아래에 박으면
				rect[i]->direction.y *= -1;													// y좌표 반대로

			//for (int j = i; j < Rect::seq; ++j) {		// 사각형끼리 충돌하면
			//	if (isCollide(rect[i], rect[j])) {		// 방향 reverse해주도록 함
			//		rect[i]->direction.x *= -1;
			//		rect[i]->direction.y *= -1;
			//		rect[j]->direction.x *= -1;
			//		rect[j]->direction.y *= -1;
			//	}
			//}

			rect[i]->move();																// 이동
		}
		break;
	case 'i':
		for (int i = 0; i < Rect::seq; ++i) {
			if (rect[i]->rect.x - rect[i]->rectSize.x / 2 <= 0 || rect[i]->rect.x + rect[i]->rectSize.x / 2 >= winWidth) {				// 왼쪽이나 오른쪽에 박으면
				rect[i]->direction.x *= -1;													// x좌표 반대로
				rect[i]->moveCount = 0;														// 카운트 초기화
			}
			else if (rect[i]->rect.y - rect[i]->rectSize.y / 2 <= 0 || rect[i]->rect.y + rect[i]->rectSize.y / 2 >= winHeight) {		// 위나 아래에 박으면
				rect[i]->direction.y *= -1;													// y좌표 반대로
				rect[i]->moveCount = 0;
			}

			if (rect[i]->moveCount == 100) {												//100번 움직이면
				rect[i]->moveCount = 0;														//초기화
				if ((rect[i]->direction.x < 0 ? -rect[i]->direction.x : rect[i]->direction.x) == 2)		//direction이 2인곳에다가 방향바꿔줌
					rect[i]->direction.x *= -1;
				else if ((rect[i]->direction.y < 0 ? -rect[i]->direction.y : rect[i]->direction.y) == 2)
					rect[i]->direction.y *= -1;
			}

			rect[i]->move();																// 이동
			++rect[i]->moveCount;
		}
		break;
	/*case 'c':
		for (int i = 0; i < Rect::seq; ++i) {
			switch (c_Count / 80) {
			case 0:
				++rect[i]->rectSize.x;
				rect[i]->rectSize.y += 2;
				break;
			case 1:
				--rect[i]->rectSize.x;
				rect[i]->rectSize.y -= 2;
				break;
			case 2:
				++rect[i]->rectSize.y;
				rect[i]->rectSize.x += 2;
				break;
			case 3:
				--rect[i]->rectSize.y;
				rect[i]->rectSize.x -= 2;
				break;
			default:
				c_Count = 0;
				break;
			}
		}
		++c_Count;
		break;*/
	}

	if (isC) {
		for (int i = 0; i < Rect::seq; ++i) {
			switch (c_Count / 80) {
			case 0:
				++rect[i]->rectSize.x;
				rect[i]->rectSize.y += 2;
				break;
			case 1:
				--rect[i]->rectSize.x;
				rect[i]->rectSize.y -= 2;
				break;
			case 2:
				++rect[i]->rectSize.y;
				rect[i]->rectSize.x += 2;
				break;
			case 3:
				--rect[i]->rectSize.y;
				rect[i]->rectSize.x -= 2;
				break;
			default:
				c_Count = 0;
				break;
			}
		}
		++c_Count;
	}

	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(timee, TimerFunction, value); // 타이머함수 재 설정
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
	data.x = (int)((1.f + x) * ((float)winWidth / 2.f));
	data.y = -(int)((-1.f + y) * ((float)winHeight / 2.f));

	return data;
}

void Rect::resetDirection()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis2(0, 3);						// 최초 방향 설정
	switch (dis2(gen)) {
	case 0:
		direction = WinXY(-1, -1);
		break;
	case 1:
		direction = WinXY(1, -1);
		break;
	case 2:
		direction = WinXY(1, 1);
		break;
	case 3:
		direction = WinXY(-1, 1);
		break;
	}
}

void Rect::init()
{
	start = rect;													// 최초 좌표 기억

	rectSize = WinXY(80, 80);										// 사각형 사이즈 지정
	moveCount = 0;

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.f, 1.f);					// 색상 설정
	for (int i = 0; i < 3; ++i)
		color[i] = dis(gen);

	resetDirection();													// 최초 방향 설정

	++seq;
}

Rect::Rect()
{
	rect = WinXY(winWidth / 2, winHeight / 2);

	init();
}

Rect::Rect(WinXY data)
{
	rect = data;

	init();
}

Rect::~Rect()
{
	--seq;
}

void Rect::move()
{
	rect.x += direction.x;
	rect.y += direction.y;
}


bool isCollide(Rect* r1, Rect* r2)
{
	if (r1->rect.x + r1->rectSize.x / 2 < r2->rect.x - r2->rectSize.x / 2 || r1->rect.x - r1->rectSize.x / 2 > r2->rect.x + r2->rectSize.x / 2) return false;
	if (r1->rect.y + r1->rectSize.y / 2 < r2->rect.y - r2->rectSize.y / 2 || r1->rect.y - r1->rectSize.y / 2 > r2->rect.y + r2->rectSize.y / 2) return false;
	return true;
}