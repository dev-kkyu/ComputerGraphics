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
public:
	WinXY rect;
	float color[3];
	WinXY rectSize;

	static int seq;

	Rect();
	Rect(int x, int y, int size);
	~Rect();
};
int Rect::seq = 0;


// �ݹ��Լ�
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

// ��ǥ��ȯ�Լ�
GLXY Win2GL(int x, int y);
WinXY GL2Win(float x, float y);

// �簢�� �浹üũ
bool isCollide(Rect* r1, Rect* r2);


// ��������
int winWidth = 1500;					// ���� â ����
int winHeight = 900;				// ���� â ����

Rect* rect[101]{};

bool MotionAble = false;

void main(int argc, char** argv)								//--- ������ ����ϰ� �ݹ��Լ� ���� 
{	//--- ������ �����ϱ�
	glutInit(&argc, argv);										// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// ���÷��� ��� ����
	glutInitWindowPosition(0, 0);							// �������� ��ġ ����
	glutInitWindowSize(winWidth, winHeight);								// �������� ũ�� ����
	glutCreateWindow("Example1");								// ������ ����(������ �̸�)

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);				// �ʱ� ���� : ȭ��Ʈ

	for (int i = 0; i < 100; ++i) {
		rect[i] = new Rect;
	}

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
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();												// �̺�Ʈ ó�� ����
}

GLvoid drawScene()												//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);						// �������� ��blue�� �� ����
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
	switch (key) {
	case 'R':
	case 'r':
		for (int i = Rect::seq; i < 100; ++i) {
			rect[i] = new Rect;
		}
		glutPostRedisplay();
		break;
	}
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		rect[Rect::seq-1] = new Rect(x, y, 80);
		MotionAble = true;
		for (int i = 0; i < Rect::seq - 1; ++i) {
			if (isCollide(rect[Rect::seq - 1], rect[i])) {
				delete rect[i];
				for (int j = i; j < Rect::seq; ++j) {
					rect[j] = rect[j + 1];
				}
			}
		}
		glutPostRedisplay();
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		delete rect[Rect::seq - 1];
		MotionAble = false;
	}
}

GLvoid Motion(int x, int y)
{
	if (MotionAble) {
		rect[Rect::seq - 1]->rect = WinXY(x, y);

		for (int i = 0; i < Rect::seq - 1; ++i) {
			if (isCollide(rect[Rect::seq - 1], rect[i])) {
				delete rect[i];
				for (int j = i; j < Rect::seq; ++j) {
					rect[j] = rect[j + 1];
				}
			}
		}

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

bool isCollide(Rect* r1, Rect* r2)
{
	if (r1->rect.x + r1->rectSize.x / 2 < r2->rect.x - r2->rectSize.x / 2 || r1->rect.x - r1->rectSize.x / 2 > r2->rect.x + r2->rectSize.x / 2) return false;
	if (r1->rect.y + r1->rectSize.y / 2 < r2->rect.y - r2->rectSize.y / 2 || r1->rect.y - r1->rectSize.y / 2 > r2->rect.y + r2->rectSize.y / 2) return false;
	return true;
}

Rect::Rect()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> disw(50, winWidth - 50);
	uniform_int_distribution<int> dish(50, winHeight - 50);

	rect = WinXY(disw(gen), dish(gen));

	uniform_real_distribution<float> dis(0.f, 1.f);
	for (int i = 0; i < 3; ++i)
		color[i] = dis(gen);

	rectSize = WinXY(40, 40);

	++seq;
}

Rect::Rect(int x, int y, int size)
{
	rect = WinXY(x, y);

	color[0] = color[1] = color[2] = 0.2f;

	rectSize = WinXY(size, size);

	++seq;
}

Rect::~Rect()
{
	--seq;
}