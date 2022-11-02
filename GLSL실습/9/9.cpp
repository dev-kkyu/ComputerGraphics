#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cmath>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

using namespace std;

GLint winWidth = 900, winHeight = 900;
float vertexData[1500][6];
int vertexCount = 0;

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
GLvoid TimerFunction(int value);

char* filetobuf(string file);

// 전역변수
GLuint VAO, VBO;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
int TimerValue = 1;

GLXY Mid;
int& RnCeta = vertexCount;
int X;


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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	InitBuffer();									// VAO, VBO 생성
	shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기

	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
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
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderID);
	//glBindVertexArray(VAO);						//VAO array를 바인드 해서 그릴거다.

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);			// 데이터 새로고침
	glLineWidth(3.0);
	//glPointSize(2.0);
	glDrawArrays(GL_LINE_STRIP, 0, vertexCount);		// 선 그리기
	//glDrawArrays(GL_POINTS, 0, vertexCount);		// 점 그리기

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

GLvoid Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		random_device rd;
		default_random_engine dre(rd());
		uniform_int_distribution<int> uid{ 1, 2 };

		switch (uid(dre)) {
		case 1:
			X = 1;
			break;
		case 2:
			X = -1;
			break;
		}

		uniform_real_distribution<float> fuid(0., 0.5);
		glClearColor(fuid(dre), fuid(dre), fuid(dre), 1.0f);

		vertexCount = 0;
		Mid = Win2GL(x, y);
		//RnCeta = 0;

		glutTimerFunc(TimerValue, TimerFunction, 1);
	}

}

GLvoid TimerFunction(int value) {
	if (RnCeta >= 360 * 4)
		return;

	float midx, midy;

	if (RnCeta >= 360 * 2) {
		float radian = (1440 - RnCeta) * 3.141592 / 180.f;

		float temp_r = 360 * 2 * 3.141592 / 180.f;
		GLXY subMid(Mid.x + (temp_r / 45 * cos(temp_r)) * 2, Mid.y);

		midx = subMid.x + -radian / 45 * cos(radian);
		midy = subMid.y + radian / 45 * sin(X * radian);
	}
	else {
		float radian = RnCeta * 3.141592 / 180.f;

		midx = Mid.x + radian / 45 * cos(radian);
		midy = Mid.y + radian / 45 * sin((-X) * radian);
	}

	float vData[6] =
	{
		midx, midy, 0.f,			1.f, 1.f, 1.f
	};

	for (int i = 0; i < 6; ++i) {
		vertexData[vertexCount][i] = vData[i];
	}

	++RnCeta;
	//++vertexCount;

	glutPostRedisplay();

	glutTimerFunc(TimerValue, TimerFunction, value);
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