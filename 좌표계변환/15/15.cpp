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
	{-1.0, 0.0, 0.0,		0.0, 1.0, 0.0},
	{1.0, 0.0, 0.0,			0.0, 1.0, 0.0},
	{0.0, -3.0, 0.0,		1.0, 0.0, 0.0},
	{0.0, 3.0, 0.0,			1.0, 0.0, 0.0},
	{0.0, 0.0, -1.0,		0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0,			0.0, 0.0, 1.0}
};

vector<glm::vec3> spinData;

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
GLint winWidth = 800, winHeight = 800;
GLuint crossVAO, crossVBO;
GLuint VAO[2], VBO[2][2], EBO[2];
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

vector<glm::vec3> vertex[2];
vector<glm::vec3> vcolor[2];
vector<glm::ivec3> face[2];

//float LradX = 0, RradX = 0;
//float LradY = 0, RradY = 0;

int TimerValue = 10;

//bool isSpin;
//bool isx, isX, isy, isY;
//bool isa, isA, isb, isB;
//bool isr, isR;

float transLX, transLY, transLZ;
float transRX, transRY, transRZ;

float plusL, plusR;
float UpDown;
float leftright;
float frontend;
bool plusMode;

class GLXY {
public:
	float x;
	float y;

	GLXY() : x(0), y(0) {}
	GLXY(float x, float y) : x(x), y(y) {}
};

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//GLXY Mid;
	//int X = 0;
	//int RnCeta = 0;
	//while (true) {
	//	if (RnCeta >= 360 * 4)
	//		return;

	//	float midx, midy;

	//	if (RnCeta >= 360 * 2) {
	//		float radian = (1440 - RnCeta) * 3.141592 / 180.f;

	//		float temp_r = 360 * 2 * 3.141592 / 180.f;
	//		GLXY subMid(Mid.x + (temp_r / 45 * cos(temp_r)) * 2, Mid.y);

	//		midx = subMid.x + -radian / 45 * cos(radian);
	//		midy = subMid.y + radian / 45 * sin(X * radian);
	//	}
	//	else {
	//		float radian = RnCeta * 3.141592 / 180.f;

	//		midx = Mid.x + radian / 45 * cos(radian);
	//		midy = Mid.y + radian / 45 * sin((-X) * radian);
	//	}n

	//	glm::vec3 vData =
	//	{
	//		midx, midy, 0.f
	//	};

	//	/*for (int i = 0; i < 6; ++i) {
	//		vertexData[vertexCount][i] = vData[i];
	//	}*/
	//	spinData.push_back(vData);

	//	++RnCeta;
	//}


	ReadObj("cube.obj", 0);
	ReadObj("cone.obj", 1);

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
	glEnable(GL_DEPTH_TEST);

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

	glm::mat4 RzSpinX = glm::mat4(1.0f);
	glm::mat4 RzSpinY = glm::mat4(1.0f);

	Tx = glm::translate(Tx, glm::vec3(leftright, UpDown, frontend));						//--- �̵� ���
	Rz = glm::rotate(Rz, glm::radians(35.f), glm::vec3(1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X
	Rz = glm::rotate(Rz, glm::radians(55.f), glm::vec3(0.0, 1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����
	scale = glm::scale(scale, glm::vec3(1.0, 1.0, 1.0));					//--- ���� ���
	TR = Tx * Rz * scale; //--- �ռ� ��ȯ ���: ���� ȸ�� �̵�
	//TR = Rz * Tx * scale; //--- �ռ� ��ȯ ���: ���� �̵� ȸ��
	//modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glDrawArrays(GL_LINES, 0, 6);

	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------����ü �׸���------------------------------------------------------------
	glBindVertexArray(VAO[0]);
	Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
	Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
	scale = glm::mat4(1.0f);		//--- ������ ��ȯ
	TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���
	RzSpinX = glm::mat4(1.0f);
	RzSpinY = glm::mat4(1.0f);

	if(plusMode)
		Tx = glm::translate(Tx, glm::vec3(-4.65 + transLX, transLY, transLZ));					//--- �̵� ���
	else
		Tx = glm::translate(Tx, glm::vec3(-0.7 + transLX+leftright, transLY + UpDown, transLZ+frontend));					//--- �̵� ���
	Rz = glm::rotate(Rz, glm::radians(35.f), glm::vec3(1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
	Rz = glm::rotate(Rz, glm::radians(55.f), glm::vec3(0.0, 1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

	//RzSpinX = glm::rotate(RzSpinX, glm::radians(LradX), glm::vec3(1.0, 0.0, 0.0));
	//RzSpinY = glm::rotate(RzSpinY, glm::radians(LradY), glm::vec3(0.0, 1.0, 0.0));

	scale = glm::scale(scale, glm::vec3(0.15 + plusL, 0.15 + plusL, 0.15 + plusL));					//--- ���� ���

	if (plusMode)
		TR = Rz * scale * RzSpinX * RzSpinY * Tx;
		//TR = Rz * RzSpinX * RzSpinY * Tx * scale;
	else
		TR = Rz * RzSpinX * RzSpinY * Tx * scale;
		//TR = Rz * Tx * RzSpinX * RzSpinY * scale;

	//TR = glm::rotate(glm::mat4(1.0f), glm::radians(35.f), glm::vec3(1.0, 0.0, 0.0))* glm::rotate(glm::mat4(1.0f), glm::radians(55.f), glm::vec3(0.0, 1.0, 0.0))* glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0f), glm::radians(LradX), glm::vec3(1.0, 0.0, 0.0)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.15, 0.15, 0.15));

	//if (isSpin)
	//	TR = Rz * Tx * scale;
	//else
	//	TR = Tx * Rz * scale; //--- �ռ� ��ȯ ���: ���� ȸ�� �̵�
	//modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	//for (int i = 0; i < face[diag1].size(); ++i)
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));
	glDrawElements(GL_TRIANGLES, face[0].size() * 3, GL_UNSIGNED_INT, 0);

	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------���� �׸���--------------------------------------------------------------
	glBindVertexArray(VAO[1]);
	Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
	Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
	scale = glm::mat4(1.0f);		//--- ������ ��ȯ
	TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���
	RzSpinX = glm::mat4(1.0f);
	RzSpinY = glm::mat4(1.0f);

	if(plusMode)
		Tx = glm::translate(Tx, glm::vec3(12.5 + transRX, transRY, transRZ));					//--- �̵� ���
	else
		Tx = glm::translate(Tx, glm::vec3(0.5 + transRX+leftright, transRY + UpDown, transRZ+frontend));					//--- �̵� ���
	Rz = glm::rotate(Rz, glm::radians(35.f), glm::vec3(1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
	Rz = glm::rotate(Rz, glm::radians(55.f), glm::vec3(0.0, 1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

	//RzSpinX = glm::rotate(RzSpinX, glm::radians(RradX), glm::vec3(1.0, 0.0, 0.0));
	//RzSpinY = glm::rotate(RzSpinY, glm::radians(RradY), glm::vec3(0.0, 1.0, 0.0));

	scale = glm::scale(scale, glm::vec3(0.04 + plusR, 0.04 + plusR, 0.04 + plusR));					//--- ���� ���

	if (plusMode)
		TR = Rz * scale * RzSpinX * RzSpinY * Tx;
		//TR = Rz * RzSpinX * RzSpinY * Tx * scale;
	else
		TR = Rz * RzSpinX * RzSpinY * Tx * scale;
		//TR = Rz * Tx * RzSpinX * RzSpinY * scale;
	
	//modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	for (int i = 0; i < face[1].size(); ++i)
		glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));




	//for (int i = 0; i < face[index].size(); ++i)
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));


	//glDrawElements(GL_TRIANGLES, face[index].size() * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * 0));		//0���ε������� ��� face ���

	glutSwapBuffers();							// ȭ�鿡 ����ϱ�
}
//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	winWidth = w;
	winHeight = h;

	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'a':
		transLX -= 0.05;
		break;
	case 'd':
		transLX += 0.05;
		break;
	case 'w':
		transLY += 0.05;
		break;
	case 's':
		transLY -= 0.05;
		break;


	case 'i':								//���� ���/Ȯ��
		plusL -= 0.005;
		break;
	case 'o':
		plusL += 0.005;
		break;
	case 'k':
		plusR -= 0.005;
		break;
	case 'l':
		plusR += 0.005;
		break;
	case 'p':
		plusMode = !plusMode;
		break;
	case 'u':
		UpDown += 0.05;
		break;
	case 'j':
		UpDown -= 0.05;
		break;

	case 'v':
		leftright += 0.05;
		break;
	case 'b':
		leftright -= 0.05;
		break;
	case 'n':
		frontend += 0.05;
		break;
	case 'm':
		frontend -= 0.05;
		break;
	}









	//switch (key) {
	//case 'x':
	//	isSpin = false;
	//	isx = !isx;
	//	break;
	//case 'X':
	//	isSpin = false;
	//	isX = !isX;
	//	break;
	//case 'y':
	//	isSpin = false;
	//	isy = !isy;
	//	break;
	//case 'Y':
	//	isSpin = false;
	//	isY = !isY;
	//	break;
	//case 'a':
	//	isSpin = false;
	//	isa = !isa;
	//	break;
	//case 'A':
	//	isSpin = false;
	//	isA = !isA;
	//	break;
	//case 'b':
	//	isSpin = false;
	//	isb = !isb;
	//	break;
	//case 'B':
	//	isSpin = false;
	//	isB = !isB;
	//	break;
	//case 'r':
	//	isSpin = true;
	//	LradX = 0, RradX = 0;
	//	LradY = 0, RradY = 0;
	//	isr = !isr;
	//	break;
	//case 'R':
	//	isSpin = true;
	//	LradX = 0, RradX = 0;
	//	LradY = 0, RradY = 0;
	//	isR = !isR;
	//	break;
	//case 's':
	//	LradX = 0, RradX = 0;
	//	LradY = 0, RradY = 0;

	//	isSpin = false;
	//	isx = isX = isy = isY = isa = isA = isb = isB = isr = isR = false;
	//	break;

	//}

	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		transRX -= 0.05;
		break;
	case GLUT_KEY_RIGHT:
		transRX += 0.05;
		break;
	case GLUT_KEY_UP:
		transRY += 0.05;
		break;
	case GLUT_KEY_DOWN:
		transRY -= 0.05;
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
	//if (isx) {
	//	LradX += 1.f;
	//}
	//if (isX) {
	//	LradX -= 1.f;
	//}
	//if (isy) {
	//	LradY += 1.f;
	//}
	//if (isY) {
	//	LradY -= 1.f;
	//}
	//if (isa) {
	//	RradX += 1.f;
	//}
	//if (isA) {
	//	RradX -= 1.f;
	//}
	//if (isb) {
	//	RradY += 1.f;
	//}
	//if (isB) {
	//	RradY -= 1.f;
	//}
	//if (isr) {
	//	LradY += 1.f;
	//	RradY += 1.f;
	//}
	//if (isR) {
	//	LradY -= 1.f;
	//	RradY -= 1.f;
	//}

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
			for (int i = -1; i < 3; ++i) {
				string word;
				getline(slice, word, ' ');
				if (i == -1) continue;						// v ������ �ǳʶڴ�

				vertemp[i] = atof(word.c_str());
				clrtemp[i] = urd(dre);
			}

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
