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

float crossData[4][6]{								// 격자 부분만 초기화
	{0.0, -1.0, 0.0,		1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0,			1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0,		0.0, 1.0, 0.0},
	{1.0, 0.0, 0.0,			0.0, 1.0, 0.0},
};

bool make_vertexShaders(int sel);
bool make_fragmentShaders();
GLuint make_shaderProgram(int sel);

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
GLuint crossVAO, crossVBO;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint crossShader;
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

vector<glm::vec3> vertex[2];
vector<glm::vec3> vcolor[2];
vector<glm::ivec3> face[2];
int index;

int selFace = 1;
int cntFace = 12;

bool isDouble;
int sel2Face;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	ReadObj("cube.obj");
	index = 1;
	ReadObj("Tet.obj");
	index = 0;

	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Example1");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);


	// 십자가 쉐이더 생성 및 버퍼 생성 그리고 데이터 삽입, 접근 규칙 정의
	crossShader = make_shaderProgram(1);

	glUseProgram(crossShader);
	glGenVertexArrays(1, &crossVAO);
	glGenBuffers(1, &crossVBO);
	glBindVertexArray(crossVAO);


	glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crossData), crossData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);


	//도형 쉐이더 생성 및 버퍼 생성
	shaderID = make_shaderProgram(2); //--- 세이더 프로그램 만들기
	InitBuffer();									// VAO, VBO 생성


	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}

bool make_vertexShaders(int sel)
{
	GLchar* vertexSource;
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	switch (sel) {
	case 1:
		vertexSource = filetobuf("vertex.glsl");
		break;
	case 2:
		vertexSource = filetobuf("Transvertex.glsl");
		break;
	}
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
GLuint make_shaderProgram(int sel)
{
	//--- 세이더 읽어와서 세이더 프로그램 만들기
	make_vertexShaders(sel); //--- 버텍스 세이더 만들기
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

void InitBuffer()					// 도형 버퍼 생성
{
	//--- VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// 정점, 색상 접근 규칙 만들기
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(crossShader);				//십자가 쉐이더 사용
	glBindVertexArray(crossVAO);			//바인드
	glDrawArrays(GL_LINES, 0, 4);			//그리기

	glUseProgram(shaderID);					//obj용 쉐이더 사용
	glBindVertexArray(VAO);					//바인드

	//정점 데이터 복사
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex[index].size() * sizeof(glm::vec3), &vertex[index][0], GL_STATIC_DRAW);

	//색상 데이터 복사
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, vcolor[index].size() * sizeof(glm::vec3), &vcolor[index][0], GL_STATIC_DRAW);

	//EBO : face 데이터 복사
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face[index].size() * sizeof(glm::ivec3), &face[index][0], GL_STATIC_DRAW);

	glm::mat4 Tx = glm::mat4(1.0f); //--- 이동 행렬 선언
	glm::mat4 Rz = glm::mat4(1.0f); //--- 회전 행렬 선언
	glm::mat4 scale = glm::mat4(1.0f); //--- 사이즈 변환
	glm::mat4 TR = glm::mat4(1.0f); //--- 합성 변환 행렬
	Tx = glm::translate(Tx, glm::vec3(-0.25, -0.25, -0.25)); //--- 이동 행렬
	Rz = glm::rotate(Rz, glm::radians(10.0f), glm::vec3(-1.0, 1.0, 0.0)); //--- 회전 행렬
	scale = glm::scale(scale, glm::vec3(1.0, 1.0, 1.0));			//--- 신축 행렬
	TR = Rz * Tx * scale; //--- 합성 변환 행렬: 신축 이동 회전
	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform 변수에 변환 값 적용하기


	glDrawElements(GL_TRIANGLES, cntFace * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (selFace - 1) * (cntFace * 3)));		//0번인덱스부터 모든 face 출력
	if (isDouble)
		glDrawElements(GL_TRIANGLES, cntFace * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (sel2Face - 1) * (cntFace * 3)));		//0번인덱스부터 모든 face 출력

	//for (int i = 0; i < face.size(); ++i) {
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));
	//}

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
	switch (key) {
	case '1':
		index = 0;
		selFace = 1;
		cntFace = 2;
		isDouble = false;
		break;
	case '2':
		index = 0;
		selFace = 2;
		cntFace = 2;
		isDouble = false;
		break;
	case '3':
		index = 0;
		selFace = 3;
		cntFace = 2;
		isDouble = false;
		break;
	case '4':
		index = 0;
		selFace = 4;
		cntFace = 2;
		isDouble = false;
		break;
	case '5':
		index = 0;
		selFace = 5;
		cntFace = 2;
		isDouble = false;
		break;
	case '6':
		index = 0;
		selFace = 6;
		cntFace = 2;
		isDouble = false;
		break;
	case '7':
		index = 1;
		selFace = 1;
		cntFace = 1;
		isDouble = false;
		break;
	case '8':
		index = 1;
		selFace = 2;
		cntFace = 1;
		isDouble = false;
		break;
	case '9':
		index = 1;
		selFace = 3;
		cntFace = 1;
		isDouble = false;
		break;
	case '0':
		index = 1;
		selFace = 4;
		cntFace = 1;
		isDouble = false;
		break;
	case 'a':
		index = 0;
		selFace = 2;
		sel2Face = 4;
		cntFace = 2;
		isDouble = true;
		break;
	case 'b':
		index = 0;
		selFace =3;
		sel2Face = 5;
		cntFace = 2;
		isDouble = true;
		break;
	case 'c':
		index = 0;
		selFace = 1;
		sel2Face = 6;
		cntFace = 2;
		isDouble = true;
		break;
	case 'e':
		index = 1;
		selFace = 1;
		sel2Face = 2;
		cntFace = 1;
		isDouble = true;
		break;
	case 'f':
		index = 1;
		selFace = 1;
		sel2Face = 3;
		cntFace = 1;
		isDouble = true;
		break;
	case 'g':
		index = 1;
		selFace = 1;
		sel2Face = 4;
		cntFace = 1;
		isDouble = true;
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
				if (i == -1) continue;						// v 읽을땐 건너뛴다

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
				if (i == -1) continue;						// f 읽을땐 건너뛴다
				if (word.find("/") != string::npos) {
					istringstream slash(word);
					string slashtmp;
					getline(slash, slashtmp, '/');

					facetemp[i] = atoi(slashtmp.c_str()) - 1;			//f 읽을땐 첫번째값만(v)	//배열인덱스 쓸거라 -1해줌
				}
				else {
					facetemp[i] = atoi(word.c_str()) - 1;			//f 읽을땐 첫번째값만(v)	//배열인덱스 쓸거라 -1해줌
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