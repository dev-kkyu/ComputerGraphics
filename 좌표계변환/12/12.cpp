#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <random>
#include <gl/glew.h>											//--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

using namespace std;

float crossData[4][6]{								// ���� �κи� �ʱ�ȭ
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

// ��������
GLint winWidth = 800, winHeight = 600;
GLuint VAO, VBO[2], EBO;
GLuint crossVAO, crossVBO;
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint crossShader;
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

vector<glm::vec3> vertex[2];
vector<glm::vec3> vcolor[2];
vector<glm::ivec3> face[2];
int index;

int selFace = 1;
int cntFace = 12;

bool isDouble;
int sel2Face;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	ReadObj("cube.obj");
	index = 1;
	ReadObj("Tet.obj");
	index = 0;

	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Example1");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);


	// ���ڰ� ���̴� ���� �� ���� ���� �׸��� ������ ����, ���� ��Ģ ����
	crossShader = make_shaderProgram(1);

	glUseProgram(crossShader);
	glGenVertexArrays(1, &crossVAO);
	glGenBuffers(1, &crossVBO);
	glBindVertexArray(crossVAO);


	glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crossData), crossData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);


	//���� ���̴� ���� �� ���� ����
	shaderID = make_shaderProgram(2); //--- ���̴� ���α׷� �����
	InitBuffer();									// VAO, VBO ����


	glutDisplayFunc(drawScene); //--- ��� �ݹ� �Լ�
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}

bool make_vertexShaders(int sel)
{
	GLchar* vertexSource;
	//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
	//--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
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
GLuint make_shaderProgram(int sel)
{
	//--- ���̴� �о�ͼ� ���̴� ���α׷� �����
	make_vertexShaders(sel); //--- ���ؽ� ���̴� �����
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

void InitBuffer()					// ���� ���� ����
{
	//--- VAO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// ����, ���� ���� ��Ģ �����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(crossShader);				//���ڰ� ���̴� ���
	glBindVertexArray(crossVAO);			//���ε�
	glDrawArrays(GL_LINES, 0, 4);			//�׸���

	glUseProgram(shaderID);					//obj�� ���̴� ���
	glBindVertexArray(VAO);					//���ε�

	//���� ������ ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex[index].size() * sizeof(glm::vec3), &vertex[index][0], GL_STATIC_DRAW);

	//���� ������ ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, vcolor[index].size() * sizeof(glm::vec3), &vcolor[index][0], GL_STATIC_DRAW);

	//EBO : face ������ ����
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face[index].size() * sizeof(glm::ivec3), &face[index][0], GL_STATIC_DRAW);

	glm::mat4 Tx = glm::mat4(1.0f); //--- �̵� ��� ����
	glm::mat4 Rz = glm::mat4(1.0f); //--- ȸ�� ��� ����
	glm::mat4 scale = glm::mat4(1.0f); //--- ������ ��ȯ
	glm::mat4 TR = glm::mat4(1.0f); //--- �ռ� ��ȯ ���
	Tx = glm::translate(Tx, glm::vec3(-0.25, -0.25, -0.25)); //--- �̵� ���
	Rz = glm::rotate(Rz, glm::radians(10.0f), glm::vec3(-1.0, 1.0, 0.0)); //--- ȸ�� ���
	scale = glm::scale(scale, glm::vec3(1.0, 1.0, 1.0));			//--- ���� ���
	TR = Rz * Tx * scale; //--- �ռ� ��ȯ ���: ���� �̵� ȸ��
	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�


	glDrawElements(GL_TRIANGLES, cntFace * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (selFace - 1) * (cntFace * 3)));		//0���ε������� ��� face ���
	if (isDouble)
		glDrawElements(GL_TRIANGLES, cntFace * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (sel2Face - 1) * (cntFace * 3)));		//0���ε������� ��� face ���

	//for (int i = 0; i < face.size(); ++i) {
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));
	//}

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
				if (i == -1) continue;						// v ������ �ǳʶڴ�

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
				if (i == -1) continue;						// f ������ �ǳʶڴ�
				if (word.find("/") != string::npos) {
					istringstream slash(word);
					string slashtmp;
					getline(slash, slashtmp, '/');

					facetemp[i] = atoi(slashtmp.c_str()) - 1;			//f ������ ù��°����(v)	//�迭�ε��� ���Ŷ� -1����
				}
				else {
					facetemp[i] = atoi(word.c_str()) - 1;			//f ������ ù��°����(v)	//�迭�ε��� ���Ŷ� -1����
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