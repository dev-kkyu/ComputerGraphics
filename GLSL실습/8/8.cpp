#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <gl/glew.h>											//--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

using namespace std;

GLint winWidth = 1300, winHeight = 900;
float vertexData[6][18];

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
};

class anglePoint {
public:
	WinXY point[3];

	anglePoint() {
		memset(point, 0, sizeof(point));
	}
	anglePoint(WinXY a1, WinXY a2, WinXY a3) {
		point[0] = a1;
		point[1] = a2;
		point[2] = a3;
	}
};

class Triangle {
	int halfsize;
public:
	WinXY point;
	WinXY direction;
	int shape;
	int speed;
	float color[9];
	
	void resetclr() {
		random_device rd;
		default_random_engine dre(rd());
		uniform_real_distribution<float> dis(0.f, 1.f);
		for (int i = 0; i < 9; ++i)
			color[i] = dis(dre);
	}
	void reset() {
		resetclr();

		random_device rd;
		default_random_engine dre(rd());
		uniform_int_distribution<int> idis(1, 4);

		halfsize = 35;
		speed = idis(dre);

		switch (idis(dre)) {
		case 1:
			direction = WinXY(1, 1);
			shape = 2;
			break;
		case 2:
			direction = WinXY(1, -1);
			shape = 2;
			break;
		case 3:
			direction = WinXY(-1, 1);
			shape = 4;
			break;
		case 4:
			direction = WinXY(-1, -1);
			shape = 4;
			break;
		}

	}
	Triangle() : point(WinXY()) {
		reset();
	}
	Triangle(WinXY point) : point(point) {
		reset();
	}
	Triangle(int inUpDown) {	// 1�̸� ���� 2�� �Ʒ���
		reset();
		switch (inUpDown) {
		case 1:
			point = WinXY(winWidth / 4 + 2 * halfsize, winHeight / 4 + halfsize);
			direction = WinXY(1, 0);
			shape = 3;
			break;
		case 2:
			point = WinXY(winWidth / 4 * 3 - 2 * halfsize, winHeight / 4 * 3 - halfsize);
			direction = WinXY(-1, 0);
			shape = 1;
			break;
		}
	}
	void move() {
		point.x += direction.x * speed;
		point.y += direction.y * speed;
	}

	anglePoint isPoint() {
		switch (shape) {		//���� ��翡 ���� ���� �������� �������ش�.
		case 1:
			return anglePoint(WinXY(point.x, point.y - halfsize * 2), WinXY(point.x - halfsize, point.y + halfsize), WinXY(point.x + halfsize, point.y + halfsize));
		case 2:
			return anglePoint(WinXY(point.x + halfsize * 2, point.y), WinXY(point.x - halfsize, point.y - halfsize), WinXY(point.x - halfsize, point.y + halfsize));
		case 3:
			return anglePoint(WinXY(point.x, point.y + halfsize * 2), WinXY(point.x - halfsize, point.y - halfsize), WinXY(point.x + halfsize, point.y - halfsize));
		case 4:
			return anglePoint(WinXY(point.x - halfsize * 2, point.y), WinXY(point.x + halfsize, point.y - halfsize), WinXY(point.x + halfsize, point.y + halfsize));
		}
	}
};

GLXY Win2GL(int x, int y);

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
GLuint VAO, VBO[2];
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
int TimerValue = 3;
bool inMove = true;
bool outMove = true;

Triangle Tri[6] = { Triangle(WinXY(winWidth / 8, winHeight / 8)), Triangle(WinXY(winWidth / 8 * 7, winHeight / 8)),
	Triangle(WinXY(winWidth / 8, winHeight / 8 * 7)), Triangle(WinXY(winWidth / 8 * 7, winHeight / 8 * 7)) ,
	Triangle(1), Triangle(2)
};


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

	float rectData[] = {
		-0.5, 0.5, 0.0,			0.0, 0.0, 0.0,
		0.5, 0.5, 0.0,			0.0, 0.0, 0.0,
		0.5, -0.5, 0.0,			0.0, 0.0, 0.0,
		-0.5, -0.5, 0.0,		0.0, 0.0, 0.0,
	};

	InitBuffer();									// VAO, VBO ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);			// �簢���� �����ؼ�
	//--- vertex data ������ �Է�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectData), rectData, GL_STATIC_DRAW);		//�簢�� ��ǥ �־��ֱ�

	shaderID = make_shaderProgram(); //--- ���̴� ���α׷� �����
	glutDisplayFunc(drawScene); //--- ��� �ݹ� �Լ�
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(TimerValue, TimerFunction, 1);
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

void BindData(GLuint Buffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

void InitBuffer()
{
	//--- VAO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//--- vertex data ������ ���� VBO ����
	glGenBuffers(2, VBO);
	glEnableVertexAttribArray(0);		// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
	glEnableVertexAttribArray(1);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderID);
	//glBindVertexArray(VAO);						//VAO array�� ���ε� �ؼ� �׸��Ŵ�.
	//glPointSize(5.0);
	//glDrawArrays(GL_POINTS, 0, 1);				//--- �������ϱ�: 0�� �ε������� 1���� ���ؽ��� ����Ͽ� �� �׸���

	BindData(VBO[1]);						// �簢�� �����س��� ���� ���ε�
	glDrawArrays(GL_LINE_LOOP, 0, 4);		// �簢�� �׸���


	float vData[6][18];
	for (int i = 0; i < 6; ++i) {					// �ﰢ�� ��ǥ �������ֱ�
		anglePoint ver = Tri[i].isPoint();
		GLXY data[3] = { Win2GL(ver.point[0].x, ver.point[0].y), Win2GL(ver.point[1].x, ver.point[1].y) , Win2GL(ver.point[2].x, ver.point[2].y) };		//GL��ǥ�� �ٲ��ش�

		float vData[18] =
		{
			data[0].x, data[0].y,	0.0,			Tri[i].color[0], Tri[i].color[1], Tri[i].color[2],
			data[1].x, data[1].y,	0.0,			Tri[i].color[3], Tri[i].color[4], Tri[i].color[5],
			data[2].x, data[2].y,	0.0,			Tri[i].color[6], Tri[i].color[7], Tri[i].color[8]
		};

		memcpy(vertexData[i], vData, sizeof(vData));			// �������� vertexData���ٰ� ��ǥ�� ����
	}

	BindData(VBO[0]);						// �ﰢ�� �����س��� ���� ���ε�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);			// �ﰢ�� ��ǥ ����
	glDrawArrays(GL_TRIANGLES, 0, 3 * 6); //--- �ﰢ�� �׸���: 0�� �ε������� 3���� ���ؽ��� ����Ͽ� �ﰢ�� �׸���

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
	case 'O':
	case 'o':
		outMove = !outMove;
		break;
	case 'I':
	case 'i':
		inMove = !inMove;
		break;
	}
}

GLvoid TimerFunction(int value) {
	float up = winHeight / 4.f;
	float down = winHeight / 4.f * 3;
	float left = winWidth / 4.f;
	float right = winWidth / 4.f * 3;

	if (outMove)
		for (int i = 0; i < 4; ++i) {	//�簢�� �װ�

			anglePoint ver = Tri[i].isPoint();						// ���� ����� �ﰢ�� �������� �޾ƿ´�.

			for (int j = 0; j < 3; ++j) {		// �� ������ ���ؼ�
				if (ver.point[j].x <= right && ver.point[j].y > up && ver.point[j].y < down) {	//���� �簢���� �����ʿ� �ε�����(��->��)
					if (ver.point[(j + 1) % 3].x > right && ver.point[(j + 2) % 3].x > right) {
						Tri[i].direction.x *= -1;		// ����
						Tri[i].shape = 2;				// �ٲ���
						break;
					}
				}

				if (ver.point[j].x >= left && ver.point[j].y > up && ver.point[j].y < down) {	//���� �簢���� ���ʿ� �ε�����(��->��)
					if (ver.point[(j + 1) % 3].x < left && ver.point[(j + 2) % 3].x < left) {
						Tri[i].direction.x *= -1;
						Tri[i].shape = 4;
						break;
					}
				}

				if (ver.point[j].y <= down && ver.point[j].x > left && ver.point[j].x < right) {	//���� �簢���� �Ʒ��ʿ� �ε�����(�Ʒ�->��)
					if (ver.point[(j + 1) % 3].y > down && ver.point[(j + 2) % 3].y > down) {
						Tri[i].direction.y *= -1;
						Tri[i].shape = 3;
						break;
					}
				}

				if (ver.point[j].y >= up && ver.point[j].x > left && ver.point[j].x < right) {	//���� �簢���� ���ʿ� �ε�����(��->�Ʒ�)
					if (ver.point[(j + 1) % 3].y < up && ver.point[(j + 2) % 3].y < up) {
						Tri[i].direction.y *= -1;
						Tri[i].shape = 1;
						break;
					}
				}

			}


			if (ver.point[0].x <= 0 || ver.point[1].x <= 0 || ver.point[2].x <= 0) {	// �ε�����,			�޺���
				Tri[i].direction.x *= -1;		// ����
				Tri[i].shape = 2;				// �ٲ���
			}
			if (ver.point[0].x >= winWidth || ver.point[1].x >= winWidth || ver.point[2].x >= winWidth) {		// ����
				Tri[i].direction.x *= -1;
				Tri[i].shape = 4;
			}
			if (ver.point[0].y <= 0 || ver.point[1].y <= 0 || ver.point[2].y <= 0) {							// ����
				Tri[i].direction.y *= -1;
				Tri[i].shape = 3;
			}
			if (ver.point[0].y >= winHeight || ver.point[1].y >= winHeight || ver.point[2].y >= winHeight) {	//�Ʒ���
				Tri[i].direction.y *= -1;
				Tri[i].shape = 1;
			}

			Tri[i].move();
		}

	if (inMove)
		for (int i = 4; i < 6; ++i) {								// ���� �簢��
			anglePoint ver = Tri[i].isPoint();						// ���� ����� �ﰢ�� �������� �޾ƿ´�.

			if (ver.point[0].x <= left || ver.point[1].x <= left || ver.point[2].x <= left) {	// �ε�����,			�޺���
				Tri[i].direction.x *= -1;		// ����
				Tri[i].resetclr();
			}
			if (ver.point[0].x >= right || ver.point[1].x >= right || ver.point[2].x >= right) {		// ����
				Tri[i].direction.x *= -1;
				Tri[i].resetclr();
			}

			Tri[i].move();
		}

	glutPostRedisplay();

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