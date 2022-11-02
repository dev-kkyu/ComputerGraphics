//#include <iostream>
//#include <random>
//#include <gl/glew.h>											//--- �ʿ��� ������� include
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
//// �ݹ��Լ�
//GLvoid drawScene(GLvoid);
//GLvoid Reshape(int w, int h);
//GLvoid Keyboard(unsigned char key, int x, int y);
//GLvoid Mouse(int button, int state, int x, int y);
//GLvoid TimerFunction(int value);
//
//// ��ǥ��ȯ�Լ�
//GLXY Win2GL(int x, int y);
//WinXY GL2Win(float x, float y);
//
//bool isCollide(Rect* r1, Rect* r2);
//
//// ��������
//int winWidth = 600;					// ���� â ����
//int winHeight = 600;				// ���� â ����
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
//void main(int argc, char** argv)								//--- ������ ����ϰ� �ݹ��Լ� ���� 
//{	//--- ������ �����ϱ�
//	glutInit(&argc, argv);										// glut �ʱ�ȭ
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// ���÷��� ��� ����
//	glutInitWindowPosition(0, 0);							// �������� ��ġ ����
//	glutInitWindowSize(winWidth, winHeight);								// �������� ũ�� ����
//	glutCreateWindow("Example1");								// ������ ����(������ �̸�)
//
//	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);				// �ʱ� ���� : £�� ȸ��
//
//	//--- GLEW �ʱ�ȭ�ϱ�
//	glewExperimental = GL_TRUE;
//	if (glewInit() != GLEW_OK)									// glew �ʱ�ȭ
//	{
//		std::cerr << "Unable to initialize GLEW" << std::endl;
//		exit(EXIT_FAILURE);
//	}
//	else
//		std::cout << "GLEW Initialized\n";
//
//	glutDisplayFunc(drawScene);									// ��� �Լ��� ����
//	glutReshapeFunc(Reshape);									// �ٽ� �׸��� �Լ� ����
//	glutKeyboardFunc(Keyboard);									// Ű���� �Է� �ݹ��Լ� ����
//	glutMouseFunc(Mouse);
//	glutMainLoop();												// �̺�Ʈ ó�� ����
//}
//
//GLvoid drawScene()												//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
//{
//	glClear(GL_COLOR_BUFFER_BIT);								// ������ ������ ��ü�� ĥ�ϱ�
//	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
//
//	for (int i = 0; i < Rect::seq; ++i) {
//		GLXY_Pair data = { Win2GL(rect[i]->rect.x - rect[i]->rectSize.x / 2, rect[i]->rect.y + rect[i]->rectSize.y / 2),Win2GL(rect[i]->rect.x + rect[i]->rectSize.x / 2, rect[i]->rect.y - rect[i]->rectSize.y / 2) };
//		glColor3f(rect[i]->color[0], rect[i]->color[1], rect[i]->color[2]);						// �ʱ� �簢�� �� : ����
//		glRectf(data.r1.x, data.r1.y, data.r2.x, data.r2.y);
//	}
//
//	glutSwapBuffers();											// ȭ�鿡 ����ϱ�
//}
//
//GLvoid Reshape(int w, int h)									//--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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
//	case 'i': 								// ������� ���� �����ֱ�
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
//		glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
//		return;
//	case 'r':
//		for (int i = Rect::seq - 1; i >= 0; --i)
//			delete rect[i];
//		c_Count = 0;
//		glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
//		return;
//	case 'q':
//		//exit(0);
//		glutLeaveMainLoop();
//	default:
//		cout << "������ �̺�Ʈ ����" << endl;
//		return;
//		
//	}
//
//	// a, i�� ���������� ����
//	lastAni = key;					// �������� �ߴ� �ִϸ��̼� ����
//	TimerValue = key;
//	/*if (isAni) {
//		TimerValue = 0;
//		isAni = false;
//	}*/
//	//else {
//		isAni = true;
//		glutTimerFunc(timee, TimerFunction, TimerValue);
//		//if (isC) {												// a, i�� �����Ϸ��� �� �� isC�� �̹� �������̸�
//		//	isStop = true;										// �ϴ� �ѹ� ���߰� ��������
//		//}
//	//}
//
//}
//
//GLvoid Mouse(int button, int state, int x, int y)				// ���� xy�� ��������ǥ��
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
//			if (rect[i]->rect.x - rect[i]->rectSize.x / 2 <= 0 || rect[i]->rect.x + rect[i]->rectSize.x / 2 >= winWidth)				// �����̳� �����ʿ� ������
//				rect[i]->direction.x *= -1;													// x��ǥ �ݴ��
//			else if (rect[i]->rect.y - rect[i]->rectSize.y / 2 <= 0 || rect[i]->rect.y + rect[i]->rectSize.y / 2 >= winHeight)		// ���� �Ʒ��� ������
//				rect[i]->direction.y *= -1;													// y��ǥ �ݴ��
//
//			for (int j = i; j < Rect::seq; ++j) {		// �簢������ �浹�ϸ�
//				if (isCollide(rect[i], rect[j])) {		// ���� reverse���ֵ��� ��
//					rect[i]->direction.x *= -1;
//					rect[i]->direction.y *= -1; 
//					rect[j]->direction.x *= -1;
//					rect[j]->direction.y *= -1;
//				}
//			}
//
//			rect[i]->move();																// �̵�
//
//			//while (true) {
//			//	switch (rect[i]->isDirection()) {			// ���⿡ ����
//			//	case 0:										// ������ ����
//			//		if (rect[i]->rect.y - 40 <= 0) {		// ���� ������
//			//			rect[i]->setDirection(3);			// �޾Ʒ���
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
//			if (rect[i]->rect.x - rect[i]->rectSize.x / 2 <= 0 || rect[i]->rect.x + rect[i]->rectSize.x / 2 >= winWidth) {				// �����̳� �����ʿ� ������
//				rect[i]->direction.x *= -1;													// x��ǥ �ݴ��
//				rect[i]->moveCount = 0;														// ī��Ʈ �ʱ�ȭ
//			}
//			else if (rect[i]->rect.y - rect[i]->rectSize.y / 2 <= 0 || rect[i]->rect.y + rect[i]->rectSize.y / 2 >= winHeight) {		// ���� �Ʒ��� ������
//				rect[i]->direction.y *= -1;													// y��ǥ �ݴ��
//				rect[i]->moveCount = 0;
//			}
//
//			if (rect[i]->moveCount == 100) {												//100�� �����̸�
//				rect[i]->moveCount = 0;														//�ʱ�ȭ
//				if ((rect[i]->direction.x < 0 ? -rect[i]->direction.x : rect[i]->direction.x) == 2)		//direction�� 2�ΰ����ٰ� ����ٲ���
//					rect[i]->direction.x *= -1;
//				else if ((rect[i]->direction.y < 0 ? -rect[i]->direction.y : rect[i]->direction.y) == 2)
//					rect[i]->direction.y *= -1;
//			}
//
//			rect[i]->move();																// �̵�
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
//	glutPostRedisplay(); // ȭ�� �� ���
//	glutTimerFunc(timee, TimerFunction, TimerValue); // Ÿ�̸��Լ� �� ����
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
//	uniform_int_distribution<int> dis2(0, 3);						// ���� ���� ����
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
//	start = rect;													// ���� ��ǥ ���
//
//	rectSize = WinXY(80, 80);										// �簢�� ������ ����
//	moveCount = 0;
//
//	random_device rd;
//	mt19937 gen(rd());
//	uniform_real_distribution<float> dis(0.f, 1.f);					// ���� ����
//	for (int i = 0; i < 3; ++i)
//		color[i] = dis(gen);
//
//	resetDirection();													// ���� ���� ����
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
#include <gl/glew.h>											//--- �ʿ��� ������� include
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


// �ݹ��Լ�
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid TimerFunction(int value);

// ��ǥ��ȯ�Լ�
GLXY Win2GL(int x, int y);
WinXY GL2Win(float x, float y);

bool isCollide(Rect* r1, Rect* r2);

// ��������
int winWidth = 600;					// ���� â ����
int winHeight = 600;				// ���� â ����

short timee = 2;
//unsigned char TimerValue = 0;
bool isAni = false;
char lastAni = 'a';
short c_Count = 0;
bool isC = false;
//bool isStop = false;

Rect* rect[10]{};

void main(int argc, char** argv)								//--- ������ ����ϰ� �ݹ��Լ� ���� 
{	//--- ������ �����ϱ�
	cout << sizeof(Rect) << endl;
	glutInit(&argc, argv);										// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// ���÷��� ��� ����
	glutInitWindowPosition(0, 0);							// �������� ��ġ ����
	glutInitWindowSize(winWidth, winHeight);								// �������� ũ�� ����
	glutCreateWindow("Example1");								// ������ ����(������ �̸�)

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);				// �ʱ� ���� : £�� ȸ��

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)									// glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);									// ��� �Լ��� ����
	glutReshapeFunc(Reshape);									// �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);									// Ű���� �Է� �ݹ��Լ� ����
	glutMouseFunc(Mouse);
	glutMainLoop();												// �̺�Ʈ ó�� ����
}

GLvoid drawScene()												//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClear(GL_COLOR_BUFFER_BIT);								// ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.

	for (int i = 0; i < Rect::seq; ++i) {
		GLXY_Pair data = { Win2GL(rect[i]->rect.x - rect[i]->rectSize.x / 2, rect[i]->rect.y + rect[i]->rectSize.y / 2),Win2GL(rect[i]->rect.x + rect[i]->rectSize.x / 2, rect[i]->rect.y - rect[i]->rectSize.y / 2) };
		glColor3f(rect[i]->color[0], rect[i]->color[1], rect[i]->color[2]);						// �ʱ� �簢�� �� : ����
		glRectf(data.r1.x, data.r1.y, data.r2.x, data.r2.y);
	}

	glutSwapBuffers();											// ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h)									//--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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
	case 'i': 								// ������� ���� �����ֱ�
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
		glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
		return;
	case 'r':
		for (int i = Rect::seq - 1; i >= 0; --i)
			delete rect[i];
		//c_Count = 0;
		glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
		return;
	case 'q':
		//exit(0);
		glutLeaveMainLoop();
	default:
		cout << "������ �̺�Ʈ ����" << endl;
		return;

	}

	// a, i�� ���������� ����
	lastAni = key;					// �������� �ߴ� �ִϸ��̼� ����
	//TimerValue = key;
	/*if (isAni) {
		TimerValue = 0;
		isAni = false;
	}*/
	//else {
	isAni = true;
	//glutTimerFunc(timee, TimerFunction, TimerValue);
	glutTimerFunc(timee, TimerFunction, key);
	//if (isC) {												// a, i�� �����Ϸ��� �� �� isC�� �̹� �������̸�
	//	isStop = true;										// �ϴ� �ѹ� ���߰� ��������
	//}
//}

}

GLvoid Mouse(int button, int state, int x, int y)				// ���� xy�� ��������ǥ��
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
			if (rect[i]->rect.x - rect[i]->rectSize.x / 2 <= 0 || rect[i]->rect.x + rect[i]->rectSize.x / 2 >= winWidth)				// �����̳� �����ʿ� ������
				rect[i]->direction.x *= -1;													// x��ǥ �ݴ��
			else if (rect[i]->rect.y - rect[i]->rectSize.y / 2 <= 0 || rect[i]->rect.y + rect[i]->rectSize.y / 2 >= winHeight)		// ���� �Ʒ��� ������
				rect[i]->direction.y *= -1;													// y��ǥ �ݴ��

			//for (int j = i; j < Rect::seq; ++j) {		// �簢������ �浹�ϸ�
			//	if (isCollide(rect[i], rect[j])) {		// ���� reverse���ֵ��� ��
			//		rect[i]->direction.x *= -1;
			//		rect[i]->direction.y *= -1;
			//		rect[j]->direction.x *= -1;
			//		rect[j]->direction.y *= -1;
			//	}
			//}

			rect[i]->move();																// �̵�
		}
		break;
	case 'i':
		for (int i = 0; i < Rect::seq; ++i) {
			if (rect[i]->rect.x - rect[i]->rectSize.x / 2 <= 0 || rect[i]->rect.x + rect[i]->rectSize.x / 2 >= winWidth) {				// �����̳� �����ʿ� ������
				rect[i]->direction.x *= -1;													// x��ǥ �ݴ��
				rect[i]->moveCount = 0;														// ī��Ʈ �ʱ�ȭ
			}
			else if (rect[i]->rect.y - rect[i]->rectSize.y / 2 <= 0 || rect[i]->rect.y + rect[i]->rectSize.y / 2 >= winHeight) {		// ���� �Ʒ��� ������
				rect[i]->direction.y *= -1;													// y��ǥ �ݴ��
				rect[i]->moveCount = 0;
			}

			if (rect[i]->moveCount == 100) {												//100�� �����̸�
				rect[i]->moveCount = 0;														//�ʱ�ȭ
				if ((rect[i]->direction.x < 0 ? -rect[i]->direction.x : rect[i]->direction.x) == 2)		//direction�� 2�ΰ����ٰ� ����ٲ���
					rect[i]->direction.x *= -1;
				else if ((rect[i]->direction.y < 0 ? -rect[i]->direction.y : rect[i]->direction.y) == 2)
					rect[i]->direction.y *= -1;
			}

			rect[i]->move();																// �̵�
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

	glutPostRedisplay(); // ȭ�� �� ���
	glutTimerFunc(timee, TimerFunction, value); // Ÿ�̸��Լ� �� ����
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
	uniform_int_distribution<int> dis2(0, 3);						// ���� ���� ����
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
	start = rect;													// ���� ��ǥ ���

	rectSize = WinXY(80, 80);										// �簢�� ������ ����
	moveCount = 0;

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.f, 1.f);					// ���� ����
	for (int i = 0; i < 3; ++i)
		color[i] = dis(gen);

	resetDirection();													// ���� ���� ����

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