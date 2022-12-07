#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <gl/glew.h>											//--- �ʿ��� ������� include
#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
//#include <gl/glm/gtc/matrix_transform.hpp>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

bool make_vertexShaders();
bool make_fragmentShaders();
GLuint make_shaderProgram();

void InitBuffer();
void InitTexture();

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid TimerFunction(int value);

char* filetobuf(string file);

// ��������
int TimerValue = 10;

GLint winWidth = 800, winHeight = 800;

GLuint VAO, VBO;
GLuint Texture;

GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

float vertexData[] = {
	//--- ��ġ //--- �븻 //--- �ؽ�ó ��ǥ
	-0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0,		0.0, 0.0,
	0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0,		1.0, 0.0,
	0.5f, 0.5f, 0.5f, 0.0, 0.0, 1.0,		1.0, 1.0,
	0.5f, 0.5f, 0.5f, 0.0, 0.0, 1.0,		1.0, 1.0,
	-0.5f, 0.5f, 0.5f, 0.0, 0.0, 1.0,		0.0, 1.0,
	-0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0,		0.0, 0.0 };


void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("�ؽ��Ļ���");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();

	//���� ���̴� ���� �� ���� ����
	shaderID = make_shaderProgram(); //--- ���̴� ���α׷� �����
	glUseProgram(shaderID);
	InitBuffer();									// VAO, VBO ����
	InitTexture();									// �ؽ��� ����

	glutDisplayFunc(drawScene); //--- ��� �ݹ� �Լ�
	glutReshapeFunc(Reshape);
	glutTimerFunc(TimerValue, TimerFunction, 1);
	glutMainLoop();
}

bool make_vertexShaders()
{
	GLchar* vertexSource;
	//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
	//--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
	vertexSource = filetobuf("vertex.glsl");
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
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//// ����, ���� ���� ��Ģ �����
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(1�� �迭 Ȱ��ȭ)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6)); //--- �ؽ�ó ��ǥ �Ӽ�
	glEnableVertexAttribArray(2);
}

void InitTexture()
{
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int ImageWidth, ImageHeight, numberOfChannel;
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	GLubyte* data = stbi_load("����¯.png", &ImageWidth, &ImageHeight, &numberOfChannel, 0);
	//GLubyte* data = stbi_load("��Ʈ.png", &ImageWidth, &ImageHeight, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glutSwapBuffers();							// ȭ�鿡 ����ϱ�
}

//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	winWidth = w;
	winHeight = h;

	glViewport(0, 0, w, h);
}

GLvoid TimerFunction(int value)
{

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

