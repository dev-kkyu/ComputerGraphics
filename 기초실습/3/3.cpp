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


// �ݹ��Լ�
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

// ��ǥ��ȯ�Լ�
GLXY Win2GL(int x, int y);
WinXY GL2Win(float x, float y);


// ��������
int winWidth = 600;					// ���� â ����
int winHeight = 600;				// ���� â ����

Rect* rect[5]{};

bool MotionAble = false;
int MotionIndex;

void main(int argc, char** argv)								//--- ������ ����ϰ� �ݹ��Լ� ���� 
{	//--- ������ �����ϱ�
	glutInit(&argc, argv);										// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// ���÷��� ��� ����
	glutInitWindowPosition(0, 0);							// �������� ��ġ ����
	glutInitWindowSize(winWidth, winHeight);								// �������� ũ�� ����
	glutCreateWindow("Example1");								// ������ ����(������ �̸�)

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);				// �ʱ� ���� : ȭ��Ʈ

	rect[Rect::seq - 1] = new Rect;

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
	glutMotionFunc(Motion);
	glutMainLoop();												// �̺�Ʈ ó�� ����
}

GLvoid drawScene()												//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);						// �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT);								// ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.

	for (int i = 0; i < Rect::seq; ++i) {
		GLXY_Pair data = { Win2GL(rect[i]->rect.x - 40, rect[i]->rect.y + 40),Win2GL(rect[i]->rect.x + 40, rect[i]->rect.y - 40)};
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
	case 'a':
		if(Rect::seq<5)
			rect[Rect::seq - 1] = new Rect;
		break;
	default:
		break;
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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
	//glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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