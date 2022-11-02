#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <gl/glew.h>											//--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

using namespace std;

GLint winWidth = 900, winHeight = 900;

float vertexData[21][6] = {
	{0.0, -1.0, 0.0,		0.0, 0.0, 0.0},				// �߰� ���μ�													//0
	{0.0, 1.0, 0.0,			0.0, 0.0, 0.0},																				//1
	{-1.0, 0.0, 0.0,		0.0, 0.0, 0.0},																				//2
	{1.0, 0.0, 0.0,			0.0, 0.0, 0.0},																				//3
																														//
	{-0.75, 0.25, 0.0,		1.0, 0.0, 1.0},				// ������, ��, �ﰢ��											//4
	{-0.25, 0.75, 0.0,		0.0, 1.0, 1.0},																				//5
	{-0.5, 0.5, 0.0,		0.0, 0.0, 1.0},																				//6
																														//
	{0.25, 0.25, 0.0,		1.0, 0.0, 0.0},				// ������ ��, �ﰢ��, �簢��									//7
	{0.5, 0.75, 0.0,		1.0, 1.0, 0.0},																				//8
	{0.75, 0.25, 0.0,		0.0, 1.0, 1.0},																				//9
	{0.5, 0.75, 0.0,		1.0, 0.0, 1.0},				//�̰� �ٲܰ�													//10
																														//
	{-0.65, -0.45, 0.0,		1.0, 1.0, 0.0},				// ���� �Ʒ�, �簢��, ������		// �̰� ��������			//11
	{-0.65, -0.75, 0.0,		0.0, 1.0, 1.0},																				//12
	{-0.5, -0.45, 0.0,		0.0, 0.0, 1.0},													// �̰� ����				//13
	{-0.35, -0.75, 0.0,		1.0, 1.0, 0.0},																				//14
	{-0.35, -0.45, 0.0,		1.0, 0.0, 1.0},													// �̰� ����������			//15
																														//
	{0.25, -0.45, 0.0,		0.0, 1.0, 1.0},				// ���� �Ʒ�, �簢��, ������		// �̰� ��������			//16
	{0.35, -0.75, 0.0,		1.0, 0.0, 1.0},																				//17
	{0.5, -0.25, 0.0,		1.0, 0.0, 1.0},													// �̰� ����				//18
	{0.65, -0.75, 0.0,		1.0, 1.0, 0.0},																				//19
	{0.75, -0.45, 0.0,		1.0, 1.0, 0.0}													// �̰� ����������			//20

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

// ��������
GLuint VAO, VBO;
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
int TimerValue = 10;
bool isTimer = false;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
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
	//--- vertex data ������ �Է�.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);		//�簢�� ��ǥ �־��ֱ�

	shaderID = make_shaderProgram(); //--- ���̴� ���α׷� �����
	glutDisplayFunc(drawScene); //--- ��� �ݹ� �Լ�
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(TimerValue, TimerFunction, 1);
	glutMainLoop();
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
	glBindVertexArray(VAO);
	//--- vertex data ������ ���� VBO ����
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//glUseProgram(shaderID);
	//glBindVertexArray(VAO);						//VAO array�� ���ε� �ؼ� �׸��Ŵ�.
	//glPointSize(5.0);
	//glDrawArrays(GL_POINTS, 0, 1);				//--- �������ϱ�: 0�� �ε������� 1���� ���ؽ��� ����Ͽ� �� �׸���


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glLineWidth(1.0f);
	glDrawArrays(GL_LINES, 0, 4);						// �� �׸���
	glLineWidth(3.0f);
	glDrawArrays(GL_LINES, 4, 2);
	glDrawArrays(GL_TRIANGLES, 4, 3);				// ��->�ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLE_STRIP, (4 + 3), 4);			// �ﰢ��->�簢�� �׸���
	glDrawArrays(GL_TRIANGLE_STRIP, (4 + 3 + 4), 5);		// �簢��->������ �׸���
	glDrawArrays(GL_TRIANGLE_STRIP, (4 + 3 + 4 + 5), 5);		// ������->�� �׸���

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
	isTimer = true;

	float tempdata[21][6] = {
	{0.0, -1.0, 0.0,		0.0, 0.0, 0.0},				// �߰� ���μ�													//0
	{0.0, 1.0, 0.0,			0.0, 0.0, 0.0},																				//1
	{-1.0, 0.0, 0.0,		0.0, 0.0, 0.0},																				//2
	{1.0, 0.0, 0.0,			0.0, 0.0, 0.0},																				//3
																														//
	{-0.75, 0.25, 0.0,		1.0, 0.0, 1.0},				// ������, ��, �ﰢ��											//4
	{-0.25, 0.75, 0.0,		0.0, 1.0, 1.0},																				//5
	{-0.5, 0.5, 0.0,		0.0, 0.0, 1.0},																				//6
																														//
	{0.25, 0.25, 0.0,		1.0, 0.0, 0.0},				// ������ ��, �ﰢ��, �簢��									//7
	{0.5, 0.75, 0.0,		1.0, 1.0, 0.0},																				//8
	{0.75, 0.25, 0.0,		0.0, 1.0, 1.0},																				//9
	{0.5, 0.75, 0.0,		1.0, 0.0, 1.0},				//�̰� �ٲܰ�													//10
																														//
	{-0.65, -0.45, 0.0,		1.0, 1.0, 0.0},				// ���� �Ʒ�, �簢��, ������		// �̰� ��������			//11
	{-0.65, -0.75, 0.0,		0.0, 1.0, 1.0},																				//12
	{-0.5, -0.45, 0.0,		0.0, 0.0, 1.0},													// �̰� ����				//13
	{-0.35, -0.75, 0.0,		1.0, 1.0, 0.0},																				//14
	{-0.35, -0.45, 0.0,		1.0, 0.0, 1.0},													// �̰� ����������			//15
																														//
	{0.25, -0.45, 0.0,		0.0, 1.0, 1.0},				// ���� �Ʒ�, �簢��, ������		// �̰� ��������			//16
	{0.35, -0.75, 0.0,		1.0, 0.0, 1.0},																				//17
	{0.5, -0.25, 0.0,		1.0, 0.0, 1.0},													// �̰� ����				//18
	{0.65, -0.75, 0.0,		1.0, 1.0, 0.0},																				//19
	{0.75, -0.45, 0.0,		1.0, 1.0, 0.0},													// �̰� ����������			//20

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