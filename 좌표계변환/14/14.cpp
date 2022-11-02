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

float crossData[6][6]{								// ���� �κи� �ʱ�ȭ
	{-1.0, 0.0, 0.0,		0.0, 1.0, 0.0},
	{1.0, 0.0, 0.0,			0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0,		1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0,			1.0, 0.0, 0.0},
	{0.0, 0.0, -1.0,		0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0,			0.0, 0.0, 1.0}
};

bool make_vertexShaders();
bool make_fragmentShaders();
GLuint make_shaderProgram();

void InitBuffer();

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid TimerFunction(int value);

char* filetobuf(string file);
void ReadObj(string file, int index);

// ��������
GLint winWidth = 800, winHeight = 800;
GLuint crossVAO, crossVBO;
GLuint VAO[4], VBO[4][2], EBO[4];
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

vector<glm::vec3> vertex[4];
vector<glm::vec3> vcolor[4];
vector<glm::ivec3> face[4];

float LradX = 0, RradX = 0;
float LradY = 0, RradY = 0;

int TimerValue = 10;

bool isSpin;
bool isx, isX, isy, isY;
bool isa, isA, isb, isB;
bool isr, isR;
int diag1 = 0;
int diag2 = 1;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	ReadObj("cube.obj", 0);
	ReadObj("cone.obj", 1);
	ReadObj("pyramid.obj", 2);
	ReadObj("geoSphere.obj", 3);

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

	//���� ���̴� ���� �� ���� ����
	shaderID = make_shaderProgram(); //--- ���̴� ���α׷� �����
	glUseProgram(shaderID);
	InitBuffer();									// VAO, VBO ����

	glutDisplayFunc(drawScene); //--- ��� �ݹ� �Լ�
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
	//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
	//--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
		vertexSource = filetobuf("Transvertex.glsl");
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

void InitBuffer()					// ���� ���� ����
{
	//--- VAO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &crossVAO);
	glGenBuffers(1, &crossVBO);

	glGenVertexArrays(4, VAO);
	for (int i = 0; i < 4; ++i)
		glGenBuffers(2, VBO[i]);
	glGenBuffers(4, EBO);

	glBindVertexArray(crossVAO);
	glBindBuffer(GL_ARRAY_BUFFER, crossVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(crossData), crossData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);


	//// ����, ���� ���� ��Ģ �����
	for (int i = 0; i < 4; ++i) {
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i][0]);
		glBufferData(GL_ARRAY_BUFFER, vertex[i].size() * sizeof(glm::vec3), &vertex[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i][1]);
		glBufferData(GL_ARRAY_BUFFER, vcolor[i].size() * sizeof(glm::vec3), &vcolor[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face[i].size() * sizeof(glm::ivec3), &face[i][0], GL_STATIC_DRAW);
	}
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Tx;
	glm::mat4 Rz;
	glm::mat4 scale;
	glm::mat4 TR;
	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform");

	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------��ǥ�� �׸���------------------------------------------------------------
	glBindVertexArray(crossVAO);					//���ε�

	Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
	Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
	scale = glm::mat4(1.0f);		//--- ������ ��ȯ
	TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���

	glm::mat4 RzSpinX = glm::mat4(1.0f);
	glm::mat4 RzSpinY = glm::mat4(1.0f);

	Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 0.0));						//--- �̵� ���
	Rz = glm::rotate(Rz, glm::radians(35.f), glm::vec3(1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X
	Rz = glm::rotate(Rz, glm::radians(55.f), glm::vec3(0.0, 1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����
	scale = glm::scale(scale, glm::vec3(1.0, 1.0, 1.0));					//--- ���� ���
	TR = Tx * Rz * scale; //--- �ռ� ��ȯ ���: ���� ȸ�� �̵�
	//TR = Rz * Tx * scale; //--- �ռ� ��ȯ ���: ���� �̵� ȸ��
	//modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glDrawArrays(GL_LINES, 0, 6);

	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------����ü �׸���------------------------------------------------------------
	glBindVertexArray(VAO[diag1]);
	Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
	Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
	scale = glm::mat4(1.0f);		//--- ������ ��ȯ
	TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���
	RzSpinX = glm::mat4(1.0f);
	RzSpinY = glm::mat4(1.0f);

	Tx = glm::translate(Tx, glm::vec3(-0.5, 0.0, 0.0));					//--- �̵� ���
	Rz = glm::rotate(Rz, glm::radians(35.f), glm::vec3(1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
	Rz = glm::rotate(Rz, glm::radians(55.f), glm::vec3(0.0, 1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

	RzSpinX = glm::rotate(RzSpinX, glm::radians(LradX), glm::vec3(1.0, 0.0, 0.0));
	RzSpinY = glm::rotate(RzSpinY, glm::radians(LradY), glm::vec3(0.0, 1.0, 0.0));

	scale = glm::scale(scale, glm::vec3(0.15, 0.15, 0.15));					//--- ���� ���

	if(isSpin)
		TR = Rz * RzSpinX * RzSpinY * Tx * scale;
	else
		TR = Rz * Tx * RzSpinX * RzSpinY * scale;

	//TR = glm::rotate(glm::mat4(1.0f), glm::radians(35.f), glm::vec3(1.0, 0.0, 0.0))* glm::rotate(glm::mat4(1.0f), glm::radians(55.f), glm::vec3(0.0, 1.0, 0.0))* glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0f), glm::radians(LradX), glm::vec3(1.0, 0.0, 0.0)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.15, 0.15, 0.15));
	
	//if (isSpin)
	//	TR = Rz * Tx * scale;
	//else
	//	TR = Tx * Rz * scale; //--- �ռ� ��ȯ ���: ���� ȸ�� �̵�
	//modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	//for (int i = 0; i < face[diag1].size(); ++i)
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));
	glDrawElements(GL_TRIANGLES, face[diag1].size() * 3, GL_UNSIGNED_INT, 0);

	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------���� �׸���--------------------------------------------------------------
	glBindVertexArray(VAO[diag2]);
	Tx = glm::mat4(1.0f);			//--- �̵� ��� ����
	Rz = glm::mat4(1.0f);			//--- ȸ�� ��� ����
	scale = glm::mat4(1.0f);		//--- ������ ��ȯ
	TR = glm::mat4(1.0f);			//--- �ռ� ��ȯ ���
	RzSpinX = glm::mat4(1.0f);
	RzSpinY = glm::mat4(1.0f);

	Tx = glm::translate(Tx, glm::vec3(0.5, 0.0, 0.0));					//--- �̵� ���
	Rz = glm::rotate(Rz, glm::radians(35.f), glm::vec3(1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X	//�����߿� , Y������ X����
	Rz = glm::rotate(Rz, glm::radians(55.f), glm::vec3(0.0, 1.0, 0.0));	//--- ȸ�� ��� -> Y	//�����߿� , Y������ X����

	RzSpinX = glm::rotate(RzSpinX, glm::radians(RradX), glm::vec3(1.0, 0.0, 0.0));
	RzSpinY = glm::rotate(RzSpinY, glm::radians(RradY), glm::vec3(0.0, 1.0, 0.0));

	scale = glm::scale(scale, glm::vec3(0.04, 0.04, 0.04));					//--- ���� ���

	if (isSpin)
		TR = Rz * RzSpinX * RzSpinY * Tx * scale;
	else
		TR = Rz * Tx * RzSpinX * RzSpinY * scale;
	//modelLocation = glGetUniformLocation(shaderID, "modelTransform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	for (int i = 0; i < face[diag2].size(); ++i)
		glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));




	//for (int i = 0; i < face[index].size(); ++i)
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));


	//glDrawElements(GL_TRIANGLES, face[index].size() * 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * 0));		//0���ε������� ��� face ���

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
	case 'x':
		isSpin = false;
		isx = !isx;
		break;
	case 'X':
		isSpin = false;
		isX = !isX;
		break;
	case 'y':
		isSpin = false;
		isy = !isy;
		break;
	case 'Y':
		isSpin = false;
		isY = !isY;
		break;
	case 'a':
		isSpin = false;
		isa = !isa;
		break;
	case 'A':
		isSpin = false;
		isA = !isA;
		break;
	case 'b':
		isSpin = false;
		isb = !isb;
		break;
	case 'B':
		isSpin = false;
		isB = !isB;
		break;
	case 'r':
		isSpin = true;
		isr = !isr;
		break;
	case 'R':
		isSpin = true;
		isR = !isR;
		break;
	case 'c':
		if (diag1 == 0)
			diag1 = 2;
		else
			diag1 = 0;
		
		if (diag2 == 1)
			diag2 = 3;
		else
			diag2 = 1;
		break;
	case 's':
		LradX = 0, RradX = 0;
		LradY = 0, RradY = 0;

		isSpin = false;
		isx = isX = isy = isY = isa = isA = isb = isB = isr = isR = false;
		diag1 = 0;
		diag2 = 1;
		break;

	}

	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key) {

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
	if (isx) {
		LradX += 1.f;
	}
	if (isX) {
		LradX -= 1.f;
	}
	if (isy) {
		LradY += 1.f;
	}
	if (isY) {
		LradY -= 1.f;
	}
	if (isa) {
		RradX += 1.f;
	}
	if (isA) {
		RradX -= 1.f;
	}
	if (isb) {
		RradY += 1.f;
	}
	if (isB) {
		RradY -= 1.f;
	}
	if (isr) {
		LradX = RradX = 0.f;
		LradY += 1.f;
		RradY += 1.f;
	}
	if (isR) {
		LradX = RradX = 0.f;
		LradY -= 1.f;
		RradY -= 1.f;
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

void ReadObj(string file, int index)
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
