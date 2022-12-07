#include "Header.h"
#include "Draw.h"
#include "Keyboard.h"
#include "KeyboardUP.h"
#include "Mouse.h"
#include "Timer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool make_vertexShaders();
bool make_fragmentShaders();
GLuint make_shaderProgram();

void InitBuffer();
void InitTexture();

GLvoid Reshape(int w, int h);

char* filetobuf(string file);
void ReadObj(string file, vector<glm::vec3>& vertexInfo);

// 전역변수
GLint winWidth = 800, winHeight = 800;

GLuint skTexture[6];

GLuint cubeVAO, cubeVBO;
GLuint pyramidVAO, pyramidVBO;
GLuint crossVAO, crossVBO;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

vector<glm::vec3> cubevertex;
vector<glm::vec3> pyramidvertex;

bool isDepTest = true;

float textureVertex[36][2]{
	{1,0},	{0,1},	{0,0},	{1,0},	{1,1},	{0,1},

	{0,0},	{1,1},	{0,1},	{0,0},	{1,0},	{1,1},

	{0,1},	{1,0},	{1,1},	{0,1},	{0,0},	{1,0},

	{1,0},	{1,1},	{0,1},	{1,0},	{0,1},	{0,0},

	{0,0},	{1,0},	{1,1},	{0,0},	{1,1},	{0,1},

	{0,0},	{1,0},	{1,1},	{0,0},	{1,1},	{0,1}
};

float triAngle[18][2]{
	{0.5, 1},
	{0, 0},
	{1, 0},

	{0.5, 1},
	{0, 0},
	{1, 0},

	{0.5, 1},
	{0, 0},
	{1, 0},

	{0.5, 1},
	{0, 0},
	{1, 0},

	{0, 1},
	{1, 0},
	{1, 1},

	{0, 1},
	{0, 0},
	{1, 0}
};


void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Example22");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//도형 쉐이더 생성 및 버퍼 생성
	shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기
	glUseProgram(shaderID);
	InitBuffer();									// VAO, VBO 생성
	InitTexture();									// 텍스쳐 생성

	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUP);
	glutSpecialFunc(SpecialKeyboard);
	glutSpecialUpFunc(SpecialKeyboardUP);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotion);
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
	ReadObj("cube.obj", cubevertex);
	ReadObj("pyramid.obj", pyramidvertex);

	//--- VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glGenVertexArrays(1, &pyramidVAO);
	glGenBuffers(1, &pyramidVBO);
	glGenVertexArrays(1, &crossVAO);
	glGenBuffers(1, &crossVBO);


	//// 정점, 색상 접근 규칙 만들기
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	glBufferData(GL_ARRAY_BUFFER, cubevertex.size() * sizeof(glm::vec3), &cubevertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);		// 버텍스 속성 배열을 사용하도록 한다.(1번 배열 활성화)

	GLuint textureVBO;
	glGenBuffers(1, &textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertex), textureVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(2);


	//// 정점, 색상 접근 규칙 만들기
	glBindVertexArray(pyramidVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
	glBufferData(GL_ARRAY_BUFFER, pyramidvertex.size() * sizeof(glm::vec3), &pyramidvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);		// 버텍스 속성 배열을 사용하도록 한다.(1번 배열 활성화)

	GLuint pyraVBO;
	glGenBuffers(1, &pyraVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pyraVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triAngle), triAngle, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(2);

	float crossVertex[6][6]{
   {-3.0, 0.0, 0.0,         0.0, 1.0, 0.0},
   { 3.0, 0.0, 0.0,         0.0, 1.0, 0.0},
   {0.0, -2.0, 0.0,         0.0, 1.0, 0.0},
   {0.0,  2.0, 0.0,         0.0, 1.0, 0.0},
   {0.0, 0.0, -2.0,         0.0, 1.0, 0.0},
   {0.0, 0.0,  2.0,         0.0, 1.0, 0.0}
	};

	//// 정점, 색상 접근 규칙 만들기
	glBindVertexArray(crossVAO);

	glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crossVertex), crossVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);		// 버텍스 속성 배열을 사용하도록 한다.(1번 배열 활성화)

}

void InitTexture()
{
	glGenTextures(6, skTexture);

	for (int i = 0; i < 6; ++i) {
		glBindTexture(GL_TEXTURE_2D, skTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		int ImageWidth, ImageHeight, numberOfChannel;
		stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
		string filename = "할로 글자.png";
		switch (i) {
		case 0:
			filename = "양갱.png";
			break;
		case 1:
			filename = "자은.png";
			break;
		case 2:
			filename = "윤수.png";
			break;
		case 3:
			filename = "꽃길세정.jpg";
			break;
		case 4:
			filename = "하트.png";
			break;
		case 5:
			filename = "하트2.png";
			break;
		}
		GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageWidth, ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}


}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	winWidth = w;
	winHeight = h;

	glViewport(0, 0, w, h);
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

void ReadObj(string file, vector<glm::vec3>& vertexInfo)
{
	vector<glm::vec3> vertex;
	vector<glm::vec3> vNormal;

	vector<glm::ivec3> vFace;
	vector<glm::ivec3> vnFace;

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
			char tmpword;
			slice >> tmpword >> vertemp.x >> vertemp.y >> vertemp.z;

			vertex.push_back(vertemp);
		}

		else if (temp[0] == 'v' && temp[1] == 'n' && temp[2] == ' ') {
			istringstream slice(temp);

			glm::vec3 vertemp;
			string tmpword;
			slice >> tmpword >> vertemp.x >> vertemp.y >> vertemp.z;

			vNormal.push_back(vertemp);
		}

		else if (temp[0] == 'f' && temp[1] == ' ') {
			istringstream slice(temp);

			glm::ivec3 vfacetemp;
			glm::ivec3 vnfacetemp;
			for (int i = -1; i < 3; ++i) {
				string word;
				getline(slice, word, ' ');
				if (i == -1) continue;						// f 읽을땐 건너뛴다
				if (word.find("/") != string::npos) {
					istringstream slash(word);
					string slashtmp;
					getline(slash, slashtmp, '/');

					vfacetemp[i] = atoi(slashtmp.c_str()) - 1;			//f 읽을땐 첫번째값만(v)	//배열인덱스 쓸거라 -1해줌

					getline(slash, slashtmp, '/');
					getline(slash, slashtmp, '/');
					vnfacetemp[i] = atoi(slashtmp.c_str()) - 1;
				}
				else {
					vfacetemp[i] = atoi(word.c_str()) - 1;			//f 읽을땐 첫번째값만(v)	//배열인덱스 쓸거라 -1해줌
				}
			}
			vFace.push_back(vfacetemp);
			vnFace.push_back(vnfacetemp);
		}
	}

	for (int i = 0; i < vFace.size(); ++i) {
		vertexInfo.push_back(vertex[vFace[i].x]);
		vertexInfo.push_back(vNormal[vnFace[i].x]);

		vertexInfo.push_back(vertex[vFace[i].y]);
		vertexInfo.push_back(vNormal[vnFace[i].y]);

		vertexInfo.push_back(vertex[vFace[i].z]);
		vertexInfo.push_back(vNormal[vnFace[i].z]);
	}
}