#include "header.h"
#include "circle.h"
#include "keyboard.h"

extern glm::mat4 mov;
extern glm::mat4 Xrotate;
extern glm::mat4 Yrotate;
extern int spinY;
extern float spinYangle;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

float crossData[6][6]{								// 격자 부분만 초기화
	{-1.0, 0.0, 0.0,		1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0,			1.0, 0.0, 0.0},
	{0.0, -1.0, 0.0,		0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0,			0.0, 1.0, 0.0},
	{0.0, 0.0, -1.0,		0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0,			0.0, 0.0, 1.0}
};
float circleData[360][6];

bool make_vertexShaders();
bool make_fragmentShaders();
GLuint make_shaderProgram();

void InitBuffer();

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid TimerFunction(int value);

char* filetobuf(string file);
void ReadObj(string file);

// 전역변수
const GLint winWidth = 800, winHeight = 800;
float wsize = 1.f, hsize = 1.f;
GLuint crossVAO, crossVBO;
GLuint cVAO, cVBO;
GLuint VAO, VBO[2], EBO;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

vector<glm::vec3> vertex;
vector<glm::vec3> rndColor;
vector<glm::vec3> vcolor[3];
vector<glm::ivec3> face;

int TimerValue = 10;

bool isDepTest = true;

Circle C1(0.0);
Circle C2(45.f);
Circle C3(-45.f);

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{

	ReadObj("geoSphere.obj");

	float radius = 0.5f;
	for (int i = 0; i < 360; ++i) {
		circleData[i][0] = radius * cos((double)i * 3.141592 / 180.f);
		circleData[i][2] = radius * sin((double)i * 3.141592 / 180.f);
	}

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

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//도형 쉐이더 생성 및 버퍼 생성
	shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기
	glUseProgram(shaderID);
	InitBuffer();									// VAO, VBO 생성

	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
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

void InitBuffer()					// 도형 버퍼 생성
{
	//--- VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &crossVAO);
	glGenBuffers(1, &crossVBO);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &cVAO);
	glGenBuffers(1, &cVBO);


	glBindVertexArray(crossVAO);
	glBindBuffer(GL_ARRAY_BUFFER, crossVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(crossData), crossData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);


	glBindVertexArray(cVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circleData), circleData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);


	//// 정점, 색상 접근 규칙 만들기
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face.size() * sizeof(glm::ivec3), &face[0], GL_STATIC_DRAW);
	
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	if (isDepTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Trans = glm::mat4(1.0f);			//--- 이동 행렬 선언
	glm::mat4 Rotate = glm::mat4(1.0f);			//--- 회전 행렬 선언
	glm::mat4 Scale = glm::mat4(1.0f);			//--- 사이즈 변환

	glm::mat4 Change = glm::mat4(1.0f);			//--- 합성 변환 행렬
	const glm::mat4 Unit = glm::mat4(1.0f);		//--- 단위 행렬 선언

	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform");


	////---------------------------------------------------------------------------------------------------------------------------
	////---------------------------------------------------------------------------------------------------------------------------
	////--------------------------------------------------좌표축 그리기------------------------------------------------------------
	//glBindVertexArray(crossVAO);					//바인드

	//Trans = glm::translate(Unit, glm::vec3(0.0, 0.0, 0.0));						//--- 이동 행렬
	//Rotate = glm::rotate(Unit, glm::radians(25.f), glm::vec3(1.0, 0.0, 0.0));	//--- 회전 행렬 -> X
	//Scale = glm::scale(Unit, glm::vec3(1.0, 1.0, 1.0));					//--- 신축 행렬

	//Change = Xrotate * mov * Yrotate;

	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기

	//glDrawArrays(GL_LINES, 0, 6);


	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------구 그리기----------------------------------------------------------------

	glBindVertexArray(VAO);

	Trans = glm::translate(Unit, glm::vec3(0.0, 0.0, 0.0));						//--- 이동 행렬
	Rotate = glm::rotate(Unit, glm::radians(20.f), glm::vec3(1.0, 0.0, 0.0));	//--- 회전 행렬 -> X
	Scale = glm::scale(Unit, glm::vec3(0.05, 0.05, 0.05));					//--- 신축 행렬

	Change = Xrotate * mov * Yrotate * Rotate * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기

	//glBufferData(GL_ARRAY_BUFFER, rndColor.size() * sizeof(glm::vec3), &rndColor[0], GL_STATIC_DRAW);
	SetColor(2);
	glDrawElements(GL_TRIANGLES, 3 * face.size(), GL_UNSIGNED_INT, (void*)0);

	C1.draw();
	C2.draw();
	C3.draw();

	glutSwapBuffers();							// 화면에 출력하기
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	//winWidth = w;
	//winHeight = h;

	wsize = w / (float)winWidth;
	hsize = h / (float)winHeight;

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-wsize, wsize, -hsize, hsize, -3.f, 3.f); //--- 투영 공간을 [-100.0, 100.0] 공간으로 설정
	unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glViewport(0, 0, w, h);
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
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
	C1.addAngle(0.5f, 1.3f);
	C2.addAngle(0.8f, 0.9f);
	C3.addAngle(1.1f, 0.5f);

	if (spinY == 1) {
		spinYangle += 1.f;
	}
	else if (spinY == -1) {
		spinYangle -= 1.f;
	}

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
			for (int i = -1; i < 3; ++i) {
				string word;
				getline(slice, word, ' ');
				if (i == -1) continue;						// v 읽을땐 건너뛴다

				vertemp[i] = atof(word.c_str());
			}
			vertex.push_back(vertemp);
			rndColor.push_back(glm::vec3(urd(dre), urd(dre), urd(dre)));
			vcolor[0].push_back(glm::vec3(0.0, 0.847, 1.0));
			vcolor[1].push_back(glm::vec3(1,0.5,0));
			//vcolor[1].push_back(glm::vec3(0.7412, 1.0, 0.071));
			vcolor[2].push_back(glm::vec3(0.855, 0.6078, 1.0));
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
			face.push_back(facetemp);
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
