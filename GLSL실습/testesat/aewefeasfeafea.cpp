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

// ��������
GLint winWidth = 800, winHeight = 600;
GLuint VAO, VBO[2], EBO;
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

vector<glm::vec3> vertex;
vector<glm::ivec3> face;

//float vertexData[12][6]{								// ���� �κи� �ʱ�ȭ
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


void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	ReadObj("diamond.objfile");
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
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //--- GL_ELEMENT_ARRAY_BUFFER ���� �������� ���ε�
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
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
	//glDrawArrays(GL_LINES, 0, 4);				// 4�и� ������
	//glLineWidth(2.0);
	//glDrawArrays(GL_LINE_LOOP, 4, 4);			// �簢�� �׸���


	//glm::mat4 Tx = glm::mat4(1.0f); //--- �̵� ��� ����
	glm::mat4 Rz = glm::mat4(1.0f); //--- ȸ�� ��� ����
	//glm::mat4 TR = glm::mat4(1.0f); //--- �ռ� ��ȯ ���
	//Tx = glm::translate(Tx, glm::vec3(-0.3, -0.3, 0.0)); //--- x������ �̵� ���
	Rz = glm::rotate(Rz, glm::radians(50.0f), glm::vec3(1.0, 1.0, 0.0)); //--- z�࿡ ���Ͽ� ȸ�� ���
	//TR = Tx * Rz; //--- �ռ� ��ȯ ���: ȸ�� �̵�
	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rz)); //--- modelTransform ������ ��ȯ �� �����ϱ�




	glDrawElements(GL_TRIANGLES, face.size() * 3, GL_UNSIGNED_INT, 0);

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
					if (i == -1) continue;						// v ������ �ǳʶڴ�

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
					if (i == -1) continue;						// f ������ �ǳʶڴ�

					facetemp[i] = atoi(string(1, word[0]).c_str()) - 1;			//f ������ ù��°����(v)	//�迭�ε��� ���Ŷ� -1����
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