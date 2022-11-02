#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <gl/glew.h>											//--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

using namespace std;

float vertexData[18];

class GLXY {
public:
	float x;
	float y;

	GLXY() {
		x = 0;
		y = 0;
	}

	GLXY(float xx, float yy) {
		x = xx;
		y = yy;
	}
};

class Triangle {
	int count;
public:
	GLXY point;
	char value;
	float color[9];
	
	void reset() {
		random_device rd;
		default_random_engine dre(rd());
		uniform_real_distribution<float> dis(0.f, 1.f);
		for (int i = 0; i < 9; ++i)
			color[i] = dis(dre);

		count = 0;
		value = 0;
	}
	Triangle() {
		point = GLXY(0.f, 0.f);
		reset();
	}
	Triangle(GLXY pnt) {
		point = pnt;
		reset();
	}
	void upNdown() {
		switch (count++ / 4) {
		case 0:
			value += 1;
			break;
		case 1:
			value -= 1;
			break;
		}

		if (count == 8)
			count = 0;
	}
};

GLXY Win2GL(int x, int y);

bool make_vertexShaders();
bool make_fragmentShaders();
GLuint make_shaderProgram();

void InitBuffer();

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);	
GLvoid Keyboard(unsigned char key, int x, int y);

char* filetobuf(string file);

// ��������
GLuint VAO, VBO;
GLint winWidth = 1000, winHeight = 600;
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

int num = 0;
bool LineXFill = true;

Triangle Tri[4] = { Triangle(Win2GL(winWidth / 4, winHeight / 4)), Triangle(Win2GL(winWidth / 4 * 3, winHeight / 4)),
	Triangle(Win2GL(winWidth / 4, winHeight / 4 * 3)), Triangle(Win2GL(winWidth / 4 * 3, winHeight / 4 * 3)) };


void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Example1");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();

	shaderID = make_shaderProgram(); //--- ���̴� ���α׷� �����
	glutDisplayFunc(drawScene); //--- ��� �ݹ� �Լ�
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLXY Win2GL(int x, int y)
{
	GLXY data;
	data.x = (float)(x - (float)winWidth / 2.0) * (float)(1.0 / (float)(winWidth / 2.0));
	data.y = -(float)(y - (float)winHeight / 2.0) * (float)(1.0 / (float)(winHeight / 2.0));

	return data;
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
	if(!result)
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

void InitBuffer()
{
	//--- VAO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//--- vertex data ������ ���� VBO ���� �� ���ε�.
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//--- vertex data ������ �Է�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	//--- ��ġ �Ӽ�: �Ӽ� ��ġ 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//--- ���� �Ӽ�: �Ӽ� ��ġ 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	GLfloat rColor, gColor, bColor;
	rColor = gColor = 1.0;
	bColor = 1.0; //--- ������ �Ķ������� ����
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderID);
	glBindVertexArray(VAO);						//VAO array�� ���ε� �ؼ� �׸��Ŵ�.
	//glPointSize(5.0);
	//glDrawArrays(GL_POINTS, 0, 1);				//--- �������ϱ�: 0�� �ε������� 1���� ���ؽ��� ����Ͽ� �� �׸���

	for (int i = 0; i < 4; ++i) {
		float value = (float)Tri[i].value * 0.02;
		float vData[18] =
		{
			Tri[i].point.x,					Tri[i].point.y + 0.2 + value,	0.0,			Tri[i].color[0], Tri[i].color[1], Tri[i].color[2],
			Tri[i].point.x - 0.1 - value,	Tri[i].point.y - 0.1 - value,	0.0,			Tri[i].color[3], Tri[i].color[4], Tri[i].color[5],
			Tri[i].point.x + 0.1 + value,	Tri[i].point.y - 0.1 - value,	0.0,			Tri[i].color[6], Tri[i].color[7], Tri[i].color[8]
		};
		memcpy(vertexData, vData, sizeof(vData));

		InitBuffer();

		int option;
		if (LineXFill) {
			option = GL_TRIANGLES;
		}
		else {
			option = GL_LINE_LOOP;
		}
		glDrawArrays(option, 0, 3); //--- �ﰢ�� �׸���: 0�� �ε������� 3���� ���ؽ��� ����Ͽ� �ﰢ�� �׸���
	}

	glutSwapBuffers();							// ȭ�鿡 ����ϱ�
}
//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	winWidth = w;
	winHeight = h;

	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Tri[num].upNdown();
		Tri[num++].point = GLXY(Win2GL(x, y));
		if (num == 4)
			num = 0;
	}
	
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		LineXFill = true;
		break;
	case 'b':
		LineXFill = false;
		break;
	case 'r':
		for (int i = 0; i < 4; ++i) {
			Tri[i].reset();
		}
		break;
	}
	glutPostRedisplay();
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