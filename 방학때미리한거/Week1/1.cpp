#include <iostream>
#include <gl/glew.h>											//--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

float* SetColor(const char* num);
int hex2num(char hex);

void main(int argc, char** argv)								//--- ������ ����ϰ� �ݹ��Լ� ���� 
{	//--- ������ �����ϱ�
	glutInit(&argc, argv);										// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// ���÷��� ��� ����
	glutInitWindowPosition(0, 0);								// �������� ��ġ ����
	glutInitWindowSize(800, 600);								// �������� ũ�� ����
	glutCreateWindow("Example1");								// ������ ����(������ �̸�)


	float* color = SetColor("36FFFF");							//�����ڵ�ǥ ���� ��������, #����ֱ�
	glClearColor(color[0], color[1], color[2], 1.0f);			//�������ϴ� ������ �����ڵ�� ����


	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)									// glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);									// ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape);									// �ٽ� �׸��� �ݹ��Լ� ����
	glutKeyboardFunc(Keyboard);									// Ű���� �Է� �ݹ��Լ� ����
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
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 3);
		switch (dis(gen)) {
		case 1:
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case 2:
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		case 3:
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		}
	}
			break;
	case 'W':
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case 'K':
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 'T':
		glutTimerFunc(100, TimerFunction, 2);
		break;
	case 'S':
		glutTimerFunc(100, TimerFunction, 0);
		break;
	case 'Q':
		exit(0);
		break;
	}

	glutPostRedisplay(); //--- ������ �ٲ𶧸��� ��� �ݹ��Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid TimerFunction(int value)			//���ʽ��� 2, �ݺ��Ҷ� 1, �����Ҷ� 0
{
	static int temp = 1;

	if (value == 2)
		temp = 1;
	else if (value == 0)
		temp = 0;

	if (temp == 0)
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 3);

	switch (dis(gen)) {
	case 1:
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 2:
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case 3:
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	}

	glutPostRedisplay(); // ȭ�� �� ���

	glutTimerFunc(100, TimerFunction, temp); // Ÿ�̸��Լ� �� ����
}

float* SetColor(const char* num)
{
	float color[3]{};

	int r = hex2num(num[0]) * 16 + hex2num(num[1]);
	int g = hex2num(num[2]) * 16 + hex2num(num[3]);
	int b = hex2num(num[4]) * 16 + hex2num(num[5]);

	color[0] = (float)r / 255.0f;
	color[1] = (float)g / 255.0f;
	color[2] = (float)b / 255.0f;

	return color;
}

int hex2num(char hex) {

	if (hex >= 48 && hex <= 57)         // ���ڰ� 0~9�̸�(ASCII �ڵ� 48~57)
	{
		// ���ڿ��� 0�� �ش��ϴ� ASCII �ڵ� ���� ��
		return hex - 48;
	}
	else if (hex >= 65 && hex <= 70)    // ���ڰ� A~F�̸�(ASCII �ڵ� 65~70)
	{                                 // �빮�ڷ� �� 16������ ó��
		// ���ڿ��� (A�� �ش��ϴ� ASCII �ڵ� �� - 10)�� ��
		return hex - (65 - 10);
	}
	else if (hex >= 97 && hex <= 102)   // ���ڰ� a~f�̸�(ASCII �ڵ� 97~102)
	{                                 // �ҹ��ڷ� �� 16������ ó��
		// ���ڿ��� (a�� �ش��ϴ� ASCII �ڵ� �� - 10)�� ��
		return hex - (97 - 10);
	}
}
