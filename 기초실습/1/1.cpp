#include <iostream>
#include <gl/glew.h>											//--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
bool TimerValue = false;

void main(int argc, char** argv)								//--- ������ ����ϰ� �ݹ��Լ� ���� 
{	//--- ������ �����ϱ�
	glutInit(&argc, argv);										// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// ���÷��� ��� ����
	glutInitWindowPosition(0, 0);							// �������� ��ġ ����
	glutInitWindowSize(800, 600);								// �������� ũ�� ����
	glutCreateWindow("Example1");								// ������ ����(������ �̸�)

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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
	//glutTimerFunc(100, TimerFunction, TimerValue);
	glutMainLoop();												// �̺�Ʈ ó�� ����
}

GLvoid drawScene()												//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//glClearColor(0.0f, 0.0f, 1.0f, 1.0f);						// �������� ��blue�� �� ����
	glClear(GL_COLOR_BUFFER_BIT);								// ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.

	glutSwapBuffers();											// ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h)									//--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'R':
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		break; //--- ������ ���������� ����
	case 'G':
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		break; //--- ������ �ʷϻ����� ����
	case 'B':
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		break; //--- ������ �Ķ������� ����
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
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid TimerFunction(int value)
{
	if (!value)
		return;

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0, 1);
	glClearColor(dis(gen), dis(gen), dis(gen), 1.0f);

	glutPostRedisplay(); // ȭ�� �� ���
	glutTimerFunc(100, TimerFunction, TimerValue); // Ÿ�̸��Լ� �� ����
}