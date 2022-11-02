#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
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

// 전역변수
GLuint VAO, VBO;
GLint winWidth = 1000, winHeight = 600;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

int num = 0;
bool LineXFill = true;

Triangle Tri[4] = { Triangle(Win2GL(winWidth / 4, winHeight / 4)), Triangle(Win2GL(winWidth / 4 * 3, winHeight / 4)),
	Triangle(Win2GL(winWidth / 4, winHeight / 4 * 3)), Triangle(Win2GL(winWidth / 4 * 3, winHeight / 4 * 3)) };


void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Example1");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기
	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
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
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
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
	//--- vertex data 저장을 위한 VBO 생성 및 바인딩.
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//--- vertex data 데이터 입력.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	//--- 위치 속성: 속성 위치 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//--- 색상 속성: 속성 위치 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	GLfloat rColor, gColor, bColor;
	rColor = gColor = 1.0;
	bColor = 1.0; //--- 배경색을 파랑색으로 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderID);
	glBindVertexArray(VAO);						//VAO array를 바인드 해서 그릴거다.
	//glPointSize(5.0);
	//glDrawArrays(GL_POINTS, 0, 1);				//--- 렌더링하기: 0번 인덱스에서 1개의 버텍스를 사용하여 점 그리기

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
		glDrawArrays(option, 0, 3); //--- 삼각형 그리기: 0번 인덱스부터 3개의 버텍스를 사용하여 삼각형 그리기
	}

	glutSwapBuffers();							// 화면에 출력하기
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
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