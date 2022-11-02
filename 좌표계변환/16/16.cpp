#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <random>
#include <gl/glew.h>											//--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;

float crossData[6][6]{								// ���� �κи� �ʱ�ȭ
	{-1.0, 0.0, 0.0,		1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0,			1.0, 0.0, 0.0},
	{0.0, -1.0, 0.0,		0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0,			0.0, 1.0, 0.0},
	{0.0, 0.0, -1.0,		0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0,			0.0, 0.0, 1.0}
};

bool make_vertexShaders();
bool make_fragmentShaders();
GLuint make_shaderProgram();

void InitBuffer();

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid TimerFunction(int value);

char* filetobuf(string file);
void ReadObj(string file, int index);

// ��������
const GLint winWidth = 800, winHeight = 800;
float wsize = 1.f, hsize = 1.f;
GLuint crossVAO, crossVBO;
GLuint VAO[2], VBO[2][2], EBO[2];
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

vector<glm::vec3> vertex[2];
vector<glm::vec3> vcolor[2];
vector<glm::ivec3> face[2];

int TimerValue = 10;

bool isDepTest = true;
bool sideOpen, frontOpen, topSpin, spinY;
float sideY, frontAngle, topAngle, yAngle;
bool pyramid, pyramidOpen;
float pyramidAngle;


void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	ReadObj("cube.obj", 0);
	ReadObj("pyramid.obj", 1);

	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Example1");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();

	//���� ���̴� ���� �� ���� ����
	shaderID = make_shaderProgram(); //--- ���̴� ���α׷� �����
	glUseProgram(shaderID);
	InitBuffer();									// VAO, VBO ����

	glutDisplayFunc(drawScene); //--- ��� �ݹ� �Լ�
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(TimerValue, TimerFunction, 1);
	glutMainLoop();
}

bool make_vertexShaders()
{
	GLchar* vertexSource;
	//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
	//--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
	vertexSource = filetobuf("Transvertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return false;
	}
}

//--- �����׸�Ʈ ���̴� ��ü �����
bool make_fragmentShaders()
{
	GLchar* fragmentSource;
	//--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
	fragmentSource = filetobuf("fragment.glsl"); // �����׼��̴� �о����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
		return false;
	}
}

//--- ���̴� ���α׷� �����
GLuint make_shaderProgram()
{
	//--- ���̴� �о�ͼ� ���̴� ���α׷� �����
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����

	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- ���̴� ���α׷� �����
	glAttachShader(ShaderProgramID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(ShaderProgramID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
	glLinkProgram(ShaderProgramID); //--- ���̴� ���α׷� ��ũ�ϱ�
	glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program ���� ����\n" << errorLog << endl;
		return false;
	}
	glUseProgram(ShaderProgramID);			//--- ������� ���̴� ���α׷� ����ϱ�
											//--- ���� ���� ���̴����α׷� ���� �� �ְ�, �� �� �Ѱ��� ���α׷��� ����Ϸ���
											//--- glUseProgram �Լ��� ȣ���Ͽ� ��� �� Ư�� ���α׷��� �����Ѵ�.
											//--- ����ϱ� ������ ȣ���� �� �ִ�.
	return ShaderProgramID;
}

void InitBuffer()					// ���� ���� ����
{
	//--- VAO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &crossVAO);
	glGenBuffers(1, &crossVBO);

	glGenVertexArrays(2, VAO);
	for (int i = 0; i < 2; ++i)
		glGenBuffers(2, VBO[i]);
	glGenBuffers(2, EBO);

	glBindVertexArray(crossVAO);
	glBindBuffer(GL_ARRAY_BUFFER, crossVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(crossData), crossData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);


	//// ����, ���� ���� ��Ģ �����
	for (int i = 0; i < 2; ++i) {
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i][0]);
		glBufferData(GL_ARRAY_BUFFER, vertex[i].size() * sizeof(glm::vec3), &vertex[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i][1]);
		glBufferData(GL_ARRAY_BUFFER, vcolor[i].size() * sizeof(glm::vec3), &vcolor[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face[i].size() * sizeof(glm::ivec3), &face[i][0], GL_STATIC_DRAW);
	}
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	if (isDepTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Tx;
	glm::mat4 Rz;
	glm::mat4 scale;
	glm::mat4 TR;

	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform");

	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------��ǥ�� �׸���------------------------------------------------------------
	glBindVertexArray(crossVAO);					//���ε�

	Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
	Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
	scale = glm::mat4(1.0f);		//--- ������ ��ȯ
	TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���

	Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 0.0));						//--- �̵� ���
	Rz = glm::rotate(Rz, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X
	Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����
	scale = glm::scale(scale, glm::vec3(1.0, 1.0, 1.0));					//--- ���� ���

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rz)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glDrawArrays(GL_LINES, 0, 6);

	glm::mat4 temp = glm::mat4(1.0f);			//--- �̵� ��� ����
	glm::mat4 Ry = glm::mat4(1.0f);



	if (!pyramid) {
		//1---------------------------------------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------------------------------------
		//--------------------------------------------------����ü �׸���------------------------------------------------------------
		glBindVertexArray(VAO[0]);

		Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
		Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
		scale = glm::mat4(1.0f);		//--- ������ ��ȯ
		TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���

		Tx = glm::translate(Tx, glm::vec3(0.0, 1.0, 0.0));						//--- �̵� ���
		Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 1.0));						//--- �̵� ���
		Rz = glm::rotate(Rz, glm::radians(frontAngle), glm::vec3(-1.0, 0.0, 0.0));
		TR = Rz * Tx;
		Tx = glm::translate(temp, glm::vec3(0.0, -1.0, 0.0));
		Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, -1.0));
		TR = Tx * TR;


		Rz = glm::rotate(temp, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
		Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

		Ry = glm::rotate(temp, glm::radians(yAngle), glm::vec3(0.0, -1.0, 0.0));
		scale = glm::scale(scale, glm::vec3(0.4, 0.3, 0.3));					//--- ���� ���

		TR = Rz * Ry * scale * TR;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 6)));


		//2---------------------------------------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------------------------------------

		glBindVertexArray(VAO[0]);
		Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
		Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
		scale = glm::mat4(1.0f);		//--- ������ ��ȯ
		TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���

		Tx = glm::translate(Tx, glm::vec3(0.0, sideY, 0.0));

		Rz = glm::rotate(temp, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
		Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

		Ry = glm::rotate(temp, glm::radians(yAngle), glm::vec3(0.0, -1.0, 0.0));

		scale = glm::scale(scale, glm::vec3(0.4, 0.3, 0.3));					//--- ���� ���

		TR = Rz * Ry * scale * Tx;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (1 * 6)));

		//3---------------------------------------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------------------------------------
		glBindVertexArray(VAO[0]);
		Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
		Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
		scale = glm::mat4(1.0f);		//--- ������ ��ȯ
		TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���

		Tx = glm::translate(Tx, glm::vec3(0.0, -1.0, 0.0));						//--- �̵� ���
		Rz = glm::rotate(Rz, glm::radians(topAngle), glm::vec3(-1.0, 0.0, 0.0));
		TR = Rz * Tx;

		Tx = glm::translate(temp, glm::vec3(0.0, 1.0, 0.0));
		TR = Tx * TR;

		Rz = glm::rotate(temp, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
		Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

		scale = glm::scale(scale, glm::vec3(0.4, 0.3, 0.3));					//--- ���� ���

		Ry = glm::rotate(temp, glm::radians(yAngle), glm::vec3(0.0, -1.0, 0.0));

		TR = Rz * Ry * scale * TR;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�


		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (2 * 6)));

		//4����---------------------------------------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------------------------------------
		glBindVertexArray(VAO[0]);
		Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
		Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
		scale = glm::mat4(1.0f);		//--- ������ ��ȯ
		TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���
		Tx = glm::translate(Tx, glm::vec3(0.0, sideY, 0.0));


		Rz = glm::rotate(temp, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
		Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

		Ry = glm::rotate(temp, glm::radians(yAngle), glm::vec3(0.0, -1.0, 0.0));

		scale = glm::scale(scale, glm::vec3(0.4, 0.3, 0.3));					//--- ���� ���

		TR = Rz * Ry * scale * Tx;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (3 * 6)));

		//5---------------------------------------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------------------------------------
		glBindVertexArray(VAO[0]);
		Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
		Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
		scale = glm::mat4(1.0f);		//--- ������ ��ȯ
		TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���

		Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 0.0));						//--- �̵� ���
		Rz = glm::rotate(Rz, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
		Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

		scale = glm::scale(scale, glm::vec3(0.4, 0.3, 0.3));					//--- ���� ���

		Ry = glm::rotate(temp, glm::radians(yAngle), glm::vec3(0.0, -1.0, 0.0));

		TR = Rz * Ry * Tx * scale;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (4 * 6)));

		//6---------------------------------------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------------------------------------
		glBindVertexArray(VAO[0]);
		Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
		Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
		scale = glm::mat4(1.0f);		//--- ������ ��ȯ
		TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���

		Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 0.0));						//--- �̵� ���
		Rz = glm::rotate(Rz, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
		Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

		scale = glm::scale(scale, glm::vec3(0.4, 0.3, 0.3));					//--- ���� ���

		Ry = glm::rotate(temp, glm::radians(yAngle), glm::vec3(0.0, -1.0, 0.0));

		TR = Rz * Ry * Tx * scale;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (5 * 6)));
	}
	else {
		//1��------------------------------------------------------------------------------------------------------
		glBindVertexArray(VAO[1]);
		Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
		Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
		scale = glm::mat4(1.0f);		//--- ������ ��ȯ
		TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���

		Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, -0.5));
		Rz = glm::rotate(temp, glm::radians(pyramidAngle), glm::vec3(1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����

		TR = Rz * Tx;

		Tx = glm::translate(temp, glm::vec3(0.0, 0.0, 0.5));

		Rz = glm::rotate(temp, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
		Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

		scale = glm::scale(scale, glm::vec3(0.6, 0.7, 0.6));					//--- ���� ���

		TR = Rz * scale * Tx * TR;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 3)));



		//2��------------------------------------------------------------------------------------------------------
		glBindVertexArray(VAO[1]);
		Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
		Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
		scale = glm::mat4(1.0f);		//--- ������ ��ȯ
		TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���

		Tx = glm::translate(Tx, glm::vec3(-0.5, 0.0, 0.0));
		Rz = glm::rotate(temp, glm::radians(pyramidAngle), glm::vec3(0.0, 0.0, -1.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����

		TR = Rz * Tx;

		Tx = glm::translate(temp, glm::vec3(0.5, 0.0, 0.0));

		Rz = glm::rotate(temp, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
		Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

		scale = glm::scale(scale, glm::vec3(0.6, 0.7, 0.6));					//--- ���� ���

		TR = Rz * scale * Tx * TR;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (1 * 3)));

		//3��------------------------------------------------------------------------------------------------------
		glBindVertexArray(VAO[1]);
		Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
		Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
		scale = glm::mat4(1.0f);		//--- ������ ��ȯ
		TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���

		Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 0.5));
		Rz = glm::rotate(temp, glm::radians(pyramidAngle), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����

		TR = Rz * Tx;

		Tx = glm::translate(temp, glm::vec3(0.0, 0.0, -0.5));

		Rz = glm::rotate(temp, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
		Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

		scale = glm::scale(scale, glm::vec3(0.6, 0.7, 0.6));					//--- ���� ���

		TR = Rz * scale * Tx * TR;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (2 * 3)));

		//4��------------------------------------------------------------------------------------------------------
		glBindVertexArray(VAO[1]);
		Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
		Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
		scale = glm::mat4(1.0f);		//--- ������ ��ȯ
		TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���

		Tx = glm::translate(Tx, glm::vec3(0.5, 0.0, 0.0));
		Rz = glm::rotate(temp, glm::radians(pyramidAngle), glm::vec3(0.0, 0.0, 1.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����

		TR = Rz * Tx;

		Tx = glm::translate(temp, glm::vec3(-0.5, 0.0, 0.0));

		Rz = glm::rotate(temp, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
		Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

		scale = glm::scale(scale, glm::vec3(0.6, 0.7, 0.6));					//--- ���� ���

		TR = Rz * scale * Tx * TR;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (3 * 3)));





		Rz = glm::rotate(temp, glm::radians(25.f), glm::vec3(-1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
		Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(0.0, -1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

		scale = glm::scale(temp, glm::vec3(0.6, 0.7, 0.6));					//--- ���� ���

		TR = Rz * scale;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(float) * (4 * 3)));
	}

	//for (int i = 0; i < face[diag1].size(); ++i)
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));

	glutSwapBuffers();							// ȭ�鿡 ����ϱ�
}
//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	//winWidth = w;
	//winHeight = h;

	wsize = w / (float)winWidth;
	hsize = h / (float)winHeight;

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-wsize, wsize, -hsize, hsize, 1.f, -1.f); //--- ���� ������ [-100.0, 100.0] �������� ����
	unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {

	pyramid = false;
	switch (key) {
	case 'h':
		isDepTest = !isDepTest;
		break;
	case 'y':
		spinY = !spinY;
		break;
	case 't':
		topSpin = true;
		break;
	case 'T':
		topSpin = false;
		break;
	case 'f':
		frontOpen = true;
		break;
	case 'F':
		frontOpen = false;
		break;
	case '1':
		sideOpen = true;
		break;
	case '2':
		sideOpen = false;
		break;
	case 'o':
		pyramid = true;
		pyramidOpen = true;
		break;
	case 'O':
		pyramid = true;
		pyramidOpen = false;
		break;
	case 'p': {
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 2.0f, -2.0f); //--- ���� ������ [-100.0, 100.0] �������� ����

		unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- ���� ��ȯ �� ����
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
			break;
	case 'P': {
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 50.f, 0.1f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0)); //--- ������ �ణ �ڷ� �̷���
		unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- ���� ��ȯ �� ����
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
			break;
	case 'r': {
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::ortho(-wsize, wsize, -hsize, hsize, 1.f, -1.f); //--- ���� ������ [-100.0, 100.0] �������� ����
		unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- ���� ��ȯ �� ����
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
			break;
	}

	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}

	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		;
}

GLvoid Motion(int x, int y)
{

	glutPostRedisplay();

}

GLvoid TimerFunction(int value)
{
	if (frontOpen) {
		if (frontAngle <= 90.f)
			frontAngle += 1.f;
	}
	else {
		if (frontAngle >= 0.f)
			frontAngle -= 1.f;
	}
	if (sideOpen) {
		if (sideY < 1.999f)
			sideY += 0.02f;
	}
	else {
		if (sideY > 0.001f)
			sideY -= 0.02f;
	}
	if (topSpin) {
		topAngle += 3.f;
	}
	else {
		topAngle = 0.0f;
	}
	if (spinY) {
		yAngle += 1.f;
	}
	else {
		yAngle = 0.0f;
	}
	if (pyramidOpen) {
		if (pyramidAngle < 232.6f)
			pyramidAngle += 1.1f;
	}
	else {
		if (pyramidAngle > 0.1f)
			pyramidAngle -= 1.1f;
	}

	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, 1);
}

char* filetobuf(string file) {
	ifstream in(file);

	string* str_buf = new string;
	while (in) {
		string temp;
		getline(in, temp);
		str_buf->append(temp);
		str_buf->append("\n");
	}

	return const_cast<char*>(str_buf->c_str());
}

//void ReadObj(string file, int index)
//{
//	ifstream in(file);
//	if (!in) {
//		cout << "OBJ File NO Have" << endl;
//		return;
//	}
//
//
//	random_device rd;
//	default_random_engine dre(rd());
//	uniform_real_distribution<float> urd{ 0.f, 1.f };
//	while (in) {
//		string temp;
//		getline(in, temp);
//
//		if (temp[0] == 'v' && temp[1] == ' ') {
//			istringstream slice(temp);
//
//			glm::vec3 vertemp;
//			glm::vec3 clrtemp;
//			for (int i = -1; i < 3; ++i) {
//				string word;
//				getline(slice, word, ' ');
//				if (i == -1) continue;						// v ������ �ǳʶڴ�
//
//				vertemp[i] = atof(word.c_str());
//				clrtemp[i] = urd(dre);
//			}
//
//			vertex[index].push_back(vertemp);
//			vcolor[index].push_back(clrtemp);
//
//		}
//
//		else if (temp[0] == 'f' && temp[1] == ' ') {
//			istringstream slice(temp);
//
//			glm::ivec3 facetemp;
//			for (int i = -1; i < 3; ++i) {
//				string word;
//				getline(slice, word, ' ');
//				if (i == -1) continue;						// f ������ �ǳʶڴ�
//				if (word.find("/") != string::npos) {
//					istringstream slash(word);
//					string slashtmp;
//					getline(slash, slashtmp, '/');
//
//					facetemp[i] = atoi(slashtmp.c_str()) - 1;			//f ������ ù��°����(v)	//�迭�ε��� ���Ŷ� -1����
//				}
//				else {
//					facetemp[i] = atoi(word.c_str()) - 1;			//f ������ ù��°����(v)	//�迭�ε��� ���Ŷ� -1����
//				}
//			}
//			face[index].push_back(facetemp);
//		}
//	}
//
//	//cout << "v information" << endl;
//	//for (int i = 0; i < vertex.size(); ++i) {
//	//	cout << vertex[i].x << ' ' << vertex[i].y << ' ' << vertex[i].z << endl;
//	//}
//	//cout << endl << "f information" << endl;;
//	//for (int i = 0; i < face.size(); ++i) {
//	//	cout << face[i].x << ' ' << face[i].y << ' ' << face[i].z << endl;
//	//}
//}



void ReadObj(string file, int index)
{
	ifstream in(file);
	if (!in) {
		cout << "OBJ File NO Have" << endl;
		return;
	}


	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> urd{ 0.f, 1.f };
	while (in) {
		string temp;
		getline(in, temp);

		if (temp[0] == 'v' && temp[1] == ' ') {
			istringstream slice(temp);

			glm::vec3 vertemp;
			glm::vec3 clrtemp;
			for (int i = 0; i < 3; ++i) {
				clrtemp[i] = urd(dre);
			}
			char tmpword;
			slice >> tmpword >> vertemp.x >> vertemp.y >> vertemp.z;

			vertex[index].push_back(vertemp);
			vcolor[index].push_back(clrtemp);

		}

		else if (temp[0] == 'f' && temp[1] == ' ') {
			istringstream slice(temp);

			glm::ivec3 facetemp;
			for (int i = -1; i < 3; ++i) {
				string word;
				getline(slice, word, ' ');
				if (i == -1) continue;						// f ������ �ǳʶڴ�
				if (word.find("/") != string::npos) {
					istringstream slash(word);
					string slashtmp;
					getline(slash, slashtmp, '/');

					facetemp[i] = atoi(slashtmp.c_str()) - 1;			//f ������ ù��°����(v)	//�迭�ε��� ���Ŷ� -1����
				}
				else {
					facetemp[i] = atoi(word.c_str()) - 1;			//f ������ ù��°����(v)	//�迭�ε��� ���Ŷ� -1����
				}
			}
			face[index].push_back(facetemp);
		}
	}

	//cout << "v information" << endl;
	//for (int i = 0; i < vertex.size(); ++i) {
	//	cout << vertex[i].x << ' ' << vertex[i].y << ' ' << vertex[i].z << endl;
	//}
	//cout << endl << "f information" << endl;;
	//for (int i = 0; i < face.size(); ++i) {
	//	cout << face[i].x << ' ' << face[i].y << ' ' << face[i].z << endl;
	//}
}