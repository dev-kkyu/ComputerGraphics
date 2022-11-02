#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <gl/glew.h>											//--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

using namespace std;

GLint winWidth = 900, winHeight = 900;

float vertexData[21][6] = {
	{0.0, -1.0, 0.0,		0.0, 0.0, 0.0},				// 중간 라인선													//0
	{0.0, 1.0, 0.0,			0.0, 0.0, 0.0},																				//1
	{-1.0, 0.0, 0.0,		0.0, 0.0, 0.0},																				//2
	{1.0, 0.0, 0.0,			0.0, 0.0, 0.0},																				//3
																														//
	{-0.75, 0.25, 0.0,		1.0, 0.0, 1.0},				// 왼쪽위, 선, 삼각형											//4
	{-0.25, 0.75, 0.0,		0.0, 1.0, 1.0},																				//5
	{-0.5, 0.5, 0.0,		0.0, 0.0, 1.0},																				//6
																														//
	{0.25, 0.25, 0.0,		1.0, 0.0, 0.0},				// 오른쪽 위, 삼각형, 사각형									//7
	{0.5, 0.75, 0.0,		1.0, 1.0, 0.0},																				//8
	{0.75, 0.25, 0.0,		0.0, 1.0, 1.0},																				//9
	{0.5, 0.75, 0.0,		1.0, 0.0, 1.0},				//이걸 바꿀거													//10
																														//
	{-0.65, -0.45, 0.0,		1.0, 1.0, 0.0},				// 왼쪽 아래, 사각형, 오각형		// 이거 왼쪽으로			//11
	{-0.65, -0.75, 0.0,		0.0, 1.0, 1.0},																				//12
	{-0.5, -0.45, 0.0,		0.0, 0.0, 1.0},													// 이거 위로				//13
	{-0.35, -0.75, 0.0,		1.0, 1.0, 0.0},																				//14
	{-0.35, -0.45, 0.0,		1.0, 0.0, 1.0},													// 이거 오른쪽으로			//15
																														//
	{0.25, -0.45, 0.0,		0.0, 1.0, 1.0},				// 왼쪽 아래, 사각형, 오각형		// 이거 왼쪽으로			//16
	{0.35, -0.75, 0.0,		1.0, 0.0, 1.0},																				//17
	{0.5, -0.25, 0.0,		1.0, 0.0, 1.0},													// 이거 위로				//18
	{0.65, -0.75, 0.0,		1.0, 1.0, 0.0},																				//19
	{0.75, -0.45, 0.0,		1.0, 1.0, 0.0}													// 이거 오른쪽으로			//20

};

bool make_vertexShaders();
bool make_fragmentShaders();
GLuint make_shaderProgram();
void BindData(GLuint Buffer);

void InitBuffer();

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

char* filetobuf(string file);

// 전역변수
GLuint VAO, VBO;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
int TimerValue = 10;
bool isTimer = false;

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
	//--- vertex data 데이터 입력.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);		//사각형 좌표 넣어주기

	shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기
	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(TimerValue, TimerFunction, 1);
	glutMainLoop();
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

	//glUseProgram(shaderID);
	//glBindVertexArray(VAO);						//VAO array를 바인드 해서 그릴거다.
	//glPointSize(5.0);
	//glDrawArrays(GL_POINTS, 0, 1);				//--- 렌더링하기: 0번 인덱스에서 1개의 버텍스를 사용하여 점 그리기


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glLineWidth(1.0f);
	glDrawArrays(GL_LINES, 0, 4);						// 선 그리기
	glLineWidth(3.0f);
	glDrawArrays(GL_LINES, 4, 2);
	glDrawArrays(GL_TRIANGLES, 4, 3);				// 선->삼각형 그리기
	glDrawArrays(GL_TRIANGLE_STRIP, (4 + 3), 4);			// 삼각형->사각형 그리기
	glDrawArrays(GL_TRIANGLE_STRIP, (4 + 3 + 4), 5);		// 사각형->오각형 그리기
	glDrawArrays(GL_TRIANGLE_STRIP, (4 + 3 + 4 + 5), 5);		// 오각형->점 그리기

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
	isTimer = true;

	float tempdata[21][6] = {
	{0.0, -1.0, 0.0,		0.0, 0.0, 0.0},				// 중간 라인선													//0
	{0.0, 1.0, 0.0,			0.0, 0.0, 0.0},																				//1
	{-1.0, 0.0, 0.0,		0.0, 0.0, 0.0},																				//2
	{1.0, 0.0, 0.0,			0.0, 0.0, 0.0},																				//3
																														//
	{-0.75, 0.25, 0.0,		1.0, 0.0, 1.0},				// 왼쪽위, 선, 삼각형											//4
	{-0.25, 0.75, 0.0,		0.0, 1.0, 1.0},																				//5
	{-0.5, 0.5, 0.0,		0.0, 0.0, 1.0},																				//6
																														//
	{0.25, 0.25, 0.0,		1.0, 0.0, 0.0},				// 오른쪽 위, 삼각형, 사각형									//7
	{0.5, 0.75, 0.0,		1.0, 1.0, 0.0},																				//8
	{0.75, 0.25, 0.0,		0.0, 1.0, 1.0},																				//9
	{0.5, 0.75, 0.0,		1.0, 0.0, 1.0},				//이걸 바꿀거													//10
																														//
	{-0.65, -0.45, 0.0,		1.0, 1.0, 0.0},				// 왼쪽 아래, 사각형, 오각형		// 이거 왼쪽으로			//11
	{-0.65, -0.75, 0.0,		0.0, 1.0, 1.0},																				//12
	{-0.5, -0.45, 0.0,		0.0, 0.0, 1.0},													// 이거 위로				//13
	{-0.35, -0.75, 0.0,		1.0, 1.0, 0.0},																				//14
	{-0.35, -0.45, 0.0,		1.0, 0.0, 1.0},													// 이거 오른쪽으로			//15
																														//
	{0.25, -0.45, 0.0,		0.0, 1.0, 1.0},				// 왼쪽 아래, 사각형, 오각형		// 이거 왼쪽으로			//16
	{0.35, -0.75, 0.0,		1.0, 0.0, 1.0},																				//17
	{0.5, -0.25, 0.0,		1.0, 0.0, 1.0},													// 이거 위로				//18
	{0.65, -0.75, 0.0,		1.0, 1.0, 0.0},																				//19
	{0.75, -0.45, 0.0,		1.0, 1.0, 0.0},													// 이거 오른쪽으로			//20

	};
	
	switch (key) {
	case'r':
	case 'R':
		memcpy(vertexData, tempdata, sizeof(tempdata));
		glutPostRedisplay();
		break;
	}
}

GLvoid TimerFunction(int value) {
	if (isTimer) {
		if (vertexData[6][0] > -0.75) {
			vertexData[6][0] -= 0.001;
			vertexData[6][1] += 0.001;
		}

		if (vertexData[10][0] < 0.75) {
			vertexData[8][0] -= 0.001;
			vertexData[10][0] += 0.001;
		}

		if (vertexData[11][0] > -0.75)
			vertexData[11][0] -= 0.0005;
		if (vertexData[13][1] < -0.25)
			vertexData[13][1] += 0.001;
		if (vertexData[15][0] < -0.25)
			vertexData[15][0] += 0.0005;

		if (vertexData[18][1] > -0.495) {
			vertexData[16][0] += 0.00025 * 3;
			vertexData[16][1] -= 0.00005 * 3;
			vertexData[17][0] += 0.00015 * 3;
			vertexData[17][1] += 0.00025 * 3;
			//vertexData[18][0]
			vertexData[18][1] -= 0.00025 * 3;
			vertexData[19][0] -= 0.00015 * 3;
			vertexData[19][1] += 0.00025 * 3;
			vertexData[20][0] -= 0.00025 * 3;
			vertexData[20][1] -= 0.00005 * 3;
		}

		glutPostRedisplay();

	}
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