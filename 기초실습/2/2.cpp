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

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);

GLXY Win2GL(int x, int y);

int winWidth = 500;					// ���� â ����
int winHeight = 500;				// ���� â ����

GLXY_Pair Rect = { {-0.5f, -0.5f}, {0.5f, 0.5f} };		// �簢�� �Ѱ��� ��ǥ��

void main(int argc, char** argv)								//--- ������ ����ϰ� �ݹ��Լ� ���� 
{	//--- ������ �����ϱ�
	glutInit(&argc, argv);										// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// ���÷��� ��� ����
	glutInitWindowPosition(0, 0);							// �������� ��ġ ����
	glutInitWindowSize(winWidth, winHeight);								// �������� ũ�� ����
	glutCreateWindow("Example1");								// ������ ����(������ �̸�)

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);				// �ʱ� ���� : ȭ��Ʈ
	glColor3f(1.0f, 0.0f, 0.0f);						// �ʱ� �簢�� �� : ����


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
	glutMouseFunc(Mouse);
	glutMainLoop();												// �̺�Ʈ ó�� ����
}

GLvoid drawScene()												//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);						// �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT);								// ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.

	glRectf(Rect.r1.x, Rect.r1.y, Rect.r2.x, Rect.r2.y);

	glutSwapBuffers();											// ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h)									//--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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
