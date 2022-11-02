#include <iostream>
#include <fstream>
#include <string>
#include <gl/glew.h>											//--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

using namespace std;


class GLXY {
public:
	float x;
	float y;

	GLXY() : x(0), y(0) {}
	GLXY(float x, float y) : x(x), y(y) {}
};

class WinXY {
public:
	int x;
	int y;

	WinXY() : x(0), y(0) {}
	WinXY(int x, int y) : x(x), y(y) {}

	WinXY operator+(const WinXY& c) const {
		WinXY temp(x + c.x, y + c.y);
		return temp;
	}

	WinXY operator-(const WinXY& c) const {
		WinXY temp(x - c.x, y - c.y);
		return temp;
	}
};

GLXY Win2GL(int x, int y);

bool make_vertexShaders();
bool make_fragmentShaders();
GLuint make_shaderProgram();

void InitBuffer();

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

char* filetobuf(string file);

bool pointInRectangle(WinXY A, WinXY B, WinXY C, WinXY D, WinXY m);
WinXY vect2d(WinXY p1, WinXY p2);


// ��������
GLint winWidth = 800, winHeight = 600;
GLuint VAO, VBO;
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

float vertexData[8][6]{								// ���� �κи� �ʱ�ȭ
	{0.0, -1.0, 0.0,		0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0,			0.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0,		0.0, 0.0, 0.0},
	{1.0, 0.0, 0.0,			0.0, 0.0, 0.0},
};

class Rect {
public:
	WinXY lu, ru, rd, ld;

	Rect() {
		lu = WinXY(winWidth / 4, winHeight / 4);
		ru = WinXY(winWidth / 4 * 3, winHeight / 4);
		rd = WinXY(winWidth / 4 * 3, winHeight / 4 * 3);
		ld = WinXY(winWidth / 4, winHeight / 4 * 3);
	}

	Rect(WinXY lu, WinXY ru, WinXY rd, WinXY ld) : lu(lu), ru(ru), rd(rd), ld(ld) {}

	Rect operator+(const WinXY& c) const {
		Rect temp(lu + c, ru + c, rd + c, ld +c);
		return temp;
	}
};


Rect rect;
Rect prevR;
WinXY now;
BYTE isLoc;


void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Example1");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();

	InitBuffer();									// VAO, VBO ����

	shaderID = make_shaderProgram(); //--- ���̴� ���α׷� �����
	glutDisplayFunc(drawScene); //--- ��� �ݹ� �Լ�
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
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

void InitBuffer()
{
	//--- VAO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//--- vertex data ������ ���� VBO ����
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	GLXY temp[4]{
		Win2GL(rect.lu.x, rect.lu.y),
		Win2GL(rect.ru.x, rect.ru.y),
		Win2GL(rect.rd.x, rect.rd.y),
		Win2GL(rect.ld.x, rect.ld.y)
	};

	float tempData[4][6]{
		{temp[0].x, temp[0].y, 0.0,		0.5, 1.0, 0.0},
		{temp[1].x, temp[1].y, 0.0,		0.0, 1.0, 1.0},
		{temp[2].x, temp[2].y, 0.0,		1.0, 0.0, 1.0},
		{temp[3].x, temp[3].y, 0.0,		0.0, 1.0, 0.5},
	};

	memcpy(vertexData + 4, tempData, sizeof(tempData));
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glLineWidth(1.0);
	glDrawArrays(GL_LINES, 0, 4);				// 4�и� ������
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_LOOP, 4, 4);			// �簢�� �׸���

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

}


GLvoid Mouse(int button, int state, int x, int y)
{
	int size = 20;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (x > rect.lu.x - size && x < rect.lu.x + size && y > rect.lu.y - size && y < rect.lu.y + size) {
			isLoc = 1;
		}
		else if (x > rect.ru.x - size && x < rect.ru.x + size && y > rect.ru.y - size && y < rect.ru.y + size) {
			isLoc = 2;
		}
		else if (x > rect.rd.x - size && x < rect.rd.x + size && y > rect.rd.y - size && y < rect.rd.y + size) {
			isLoc = 3;
		}
		else if (x > rect.ld.x - size && x < rect.ld.x + size && y > rect.ld.y - size && y < rect.ld.y + size) {
			isLoc = 4;
		}
		else if (pointInRectangle(rect.lu, rect.ld, rect.rd, rect.ru, WinXY(x, y))) {
			isLoc = 5;
			prevR = rect;
			now = WinXY(x, y);
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		isLoc = 0;
}

GLvoid Motion(int x, int y)
{
	WinXY change(x, y);

	switch (isLoc) {
	case 1:
		rect.lu = change;
		break;
	case 2:
		rect.ru = change;
		break;
	case 3:
		rect.rd = change;
		break;
	case 4:
		rect.ld = change;
		break;
	case 5:
		rect = prevR + (change - now);
		break;
	default:
		return;
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

bool pointInRectangle(WinXY A, WinXY B, WinXY C, WinXY D, WinXY m) {		//����, �޾�, ����, ���� ����
	WinXY AB = vect2d(A, B);  float C1 = -1 * (AB.y * A.x + AB.x * A.y); float D1 = (AB.y * m.x + AB.x * m.y) + C1;
	WinXY AD = vect2d(A, D);  float C2 = -1 * (AD.y * A.x + AD.x * A.y); float D2 = (AD.y * m.x + AD.x * m.y) + C2;
	WinXY BC = vect2d(B, C);  float C3 = -1 * (BC.y * B.x + BC.x * B.y); float D3 = (BC.y * m.x + BC.x * m.y) + C3;
	WinXY CD = vect2d(C, D);  float C4 = -1 * (CD.y * C.x + CD.x * C.y); float D4 = (CD.y * m.x + CD.x * m.y) + C4;
	return 0 >= D1 && 0 >= D4 && 0 <= D2 && 0 >= D3;
}


WinXY vect2d(WinXY p1, WinXY p2) {
	return WinXY(p2.x - p1.x, -1 * (p2.y - p1.y));
}
