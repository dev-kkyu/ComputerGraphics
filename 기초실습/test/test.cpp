//#include <iostream>
//#include <gl/glew.h>											//--- �ʿ��� ������� include
//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//GLvoid drawScene(GLvoid);
//GLvoid Reshape(int w, int h);
//GLvoid Keyboard(BYTE key, int x, int y);
//GLvoid Timer(int value);
//
//int timervalue = 0;
//bool stopTimer = false;
//int timerspeed = 1000;
//
//void main(int argc, char** argv)								//--- ������ ����ϰ� �ݹ��Լ� ���� 
//{	//--- ������ �����ϱ�
//	glutInit(&argc, argv);										// glut �ʱ�ȭ
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// ���÷��� ��� ����
//	glutInitWindowPosition(100, 100);							// �������� ��ġ ����
//	glutInitWindowSize(250, 250);								// �������� ũ�� ����
//	glutCreateWindow("Example1");								// ������ ����(������ �̸�)
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
//	glutKeyboardFunc(Keyboard);
//	glutMainLoop();												// �̺�Ʈ ó�� ����
//}
//
//GLvoid drawScene()												//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
//{
//	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);						// �������� ��blue�� �� ����
//	glClear(GL_COLOR_BUFFER_BIT);								// ������ ������ ��ü�� ĥ�ϱ�
//	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
//
//	glutSwapBuffers();											// ȭ�鿡 ����ϱ�
//}
//
//GLvoid Reshape(int w, int h)									//--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
//{
//	glViewport(0, 0, w, h);
//}
//
//GLvoid Keyboard(BYTE key, int x, int y)
//{
//	stopTimer = false;
//	if (key == 'a' || key == 'q' || key == 's') {
//	timervalue = key;
//	}
//
//	glutTimerFunc(timerspeed, Timer, timervalue);
//
//	/*if (key == 'q')
//		stopTimer = true;
//
//	glutTimerFunc(timerspeed, Timer, timervalue);*/
//}
//
//GLvoid Timer(int value)
//{
//	/*if (stopTimer)
//		return;*/
//	if (value == 'q')
//		return;
//
//	if (value == 'a')
//		std::cout << "a" << std::endl;
//	if (value == 's')
//		std::cout << "s" << std::endl;
//	
//
//
//	glutTimerFunc(timerspeed, Timer, timervalue);
//}
//
