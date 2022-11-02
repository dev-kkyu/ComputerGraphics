#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <random>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>


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
void ReadObj(string file);

// 전역변수
GLint winWidth = 800, winHeight = 600;
GLuint VAO, VBO[2], EBO;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

vector<glm::vec3> vertex;
vector<glm::ivec3> face;

//float vertexData[12][6]{								// 격자 부분만 초기화
//	{0.0, -1.0, 0.0,		1.0f, 0.0f, 0.0f},
//	{0.0, 1.0, 0.0,			1.0f, 0.0f, 0.0f},
//	{-1.0, 0.0, 0.0,		0.0f, 1.0f, 0.0f},
//	{1.0, 0.0, 0.0,			0.0f, 1.0f, 0.0f},
//
//	//{0.0, 0.0, 0.0,			0.0f, 1.0f, 0.0f},
//	//{0.5, 0.0, 0.0,			0.0f, 1.0f, 0.0f},
//	//{0.5, 0.0, 0.5,			0.0f, 1.0f, 0.0f},
//	//{0.0, 0.0, 0.5,			0.0f, 1.0f, 0.0f},
//	//{0.0, 0.5, 0.0,			0.0f, 1.0f, 0.0f},
//	//{0.5, 0.5, 0.0,			0.0f, 1.0f, 0.0f},
//	//{0.5, 0.5, 0.5,			0.0f, 1.0f, 0.0f},
//	//{0.0, 0.5, 0.5,			0.0f, 1.0f, 0.0f},
//};


void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	ReadObj("diamond.objfile");
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
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //--- GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, vcolor.size() * sizeof(glm::vec3), &vcolor[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face.size() * sizeof(glm::ivec3), &face[0], GL_STATIC_DRAW);

	//glLineWidth(1.0);
	//glDrawArrays(GL_LINES, 0, 4);				// 4분면 나누기
	//glLineWidth(2.0);
	//glDrawArrays(GL_LINE_LOOP, 4, 4);			// 사각형 그리기


	//glm::mat4 Tx = glm::mat4(1.0f); //--- 이동 행렬 선언
	glm::mat4 Rz = glm::mat4(1.0f); //--- 회전 행렬 선언
	//glm::mat4 TR = glm::mat4(1.0f); //--- 합성 변환 행렬
	//Tx = glm::translate(Tx, glm::vec3(-0.3, -0.3, 0.0)); //--- x축으로 이동 행렬
	Rz = glm::rotate(Rz, glm::radians(50.0f), glm::vec3(1.0, 1.0, 0.0)); //--- z축에 대하여 회전 행렬
	//TR = Tx * Rz; //--- 합성 변환 행렬: 회전 이동
	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rz)); //--- modelTransform 변수에 변환 값 적용하기




	glDrawElements(GL_TRIANGLES, face.size() * 3, GL_UNSIGNED_INT, 0);

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

	void ReadObj(string file)
	{
		ifstream in(file);
		if (!in) {
			cout << "OBJ File NO Have" << endl;
			return;
		}

		while (in) {
			string temp;
			getline(in, temp);

			if (temp[0] == 'v' && temp[1] == ' ') {
				istringstream slice(temp);

				glm::vec3 vertemp;
				for (int i = -1; i < 3; ++i) {
					string word;
					getline(slice, word, ' ');
					if (i == -1) continue;						// v 읽을땐 건너뛴다

					vertemp[i] = atof(word.c_str());
				}
				vertex.push_back(vertemp);
			}

			else if (temp[0] == 'f' && temp[1] == ' ') {
				istringstream slice(temp);

				glm::ivec3 facetemp;
				for (int i = -1; i < 3; ++i) {
					string word;
					getline(slice, word, ' ');
					if (i == -1) continue;						// f 읽을땐 건너뛴다

					facetemp[i] = atoi(string(1, word[0]).c_str()) - 1;			//f 읽을땐 첫번째값만(v)	//배열인덱스 쓸거라 -1해줌
				}
				face.push_back(facetemp);
			}
		}

		cout << "v information" << endl;
		for (int i = 0; i < vertex.size(); ++i) {
			cout << vertex[i].x << ' ' << vertex[i].y << ' ' << vertex[i].z << endl;
		}
		cout << endl << "f information" << endl;;
		for (int i = 0; i < face.size(); ++i) {
			cout << face[i].x << ' ' << face[i].y << ' ' << face[i].z << endl;
		}
	}