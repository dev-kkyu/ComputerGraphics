#include <iostream>
#include <fstream>
#include <string>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
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


// 전역변수
GLint winWidth = 800, winHeight = 600;
GLuint VAO, VBO;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

float vertexData[8][6]{								// 격자 부분만 초기화
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


void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Example1");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	InitBuffer();									// VAO, VBO 생성

	shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기
	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
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
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
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
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return false;
	}
}

//--- 프래그먼트 세이더 객체 만들기
bool make_fragmentShaders()
{
	GLchar* fragmentSource;
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return false;
	}
}

//--- 세이더 프로그램 만들기
GLuint make_shaderProgram()
{
	//--- 세이더 읽어와서 세이더 프로그램 만들기
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기

	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기
	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		return false;
	}
	glUseProgram(ShaderProgramID);			//--- 만들어진 세이더 프로그램 사용하기
											//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
											//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
											//--- 사용하기 직전에 호출할 수 있다.
	return ShaderProgramID;
}

void InitBuffer()
{
	//--- VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//--- vertex data 저장을 위한 VBO 생성
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
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
	glDrawArrays(GL_LINES, 0, 4);				// 4분면 나누기
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_LOOP, 4, 4);			// 사각형 그리기

	glutSwapBuffers();							// 화면에 출력하기
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
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

bool pointInRectangle(WinXY A, WinXY B, WinXY C, WinXY D, WinXY m) {		//왼위, 왼아, 오아, 오위 순서
	WinXY AB = vect2d(A, B);  float C1 = -1 * (AB.y * A.x + AB.x * A.y); float D1 = (AB.y * m.x + AB.x * m.y) + C1;
	WinXY AD = vect2d(A, D);  float C2 = -1 * (AD.y * A.x + AD.x * A.y); float D2 = (AD.y * m.x + AD.x * m.y) + C2;
	WinXY BC = vect2d(B, C);  float C3 = -1 * (BC.y * B.x + BC.x * B.y); float D3 = (BC.y * m.x + BC.x * m.y) + C3;
	WinXY CD = vect2d(C, D);  float C4 = -1 * (CD.y * C.x + CD.x * C.y); float D4 = (CD.y * m.x + CD.x * m.y) + C4;
	return 0 >= D1 && 0 >= D4 && 0 <= D2 && 0 >= D3;
}


WinXY vect2d(WinXY p1, WinXY p2) {
	return WinXY(p2.x - p1.x, -1 * (p2.y - p1.y));
}
