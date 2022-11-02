#include<fstream>
#include<vector>
#include<iostream>
#include<cmath>
#include<random>
#include<gl/glew.h>
#include<gl/freeglut.h>

using namespace std;

char* filetobuf(const string filename);

void DrawSceneCall();
void ReshapeCall(int w, int h);
void keyboardCall(unsigned char key, int x, int y);
void mouseCall(int button, int state, int x, int y);
void timerCall(int value);

void makeVertexShader();
void makeFragmentShader();
void makeShaderID();
void InitBuffer();


GLuint vertexShader;
GLuint fragmentShader;
GLuint ShaderID;

char* vertexSource;
char* fragmentSource;

GLuint VAO;
GLuint VBO[2];

int Wwidth = 800;
int Wheight = 600;

int theta = 0;
float radious = 0.1;
int roundConter = 0;
bool modeChange = false;
vector<float> dots;

float start[3];
bool click = false;
bool reversemode = false;
bool dotLine = true;

float backgraoundColor[3] = { 0 };

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(Wwidth, Wheight);
    glutCreateWindow("½Ç½À 9");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        cerr << "fail Initialize" << endl;
    else cout << "Initialize" << endl;

    makeShaderID();
    glUseProgram(ShaderID);
    glutDisplayFunc(DrawSceneCall);
    glutReshapeFunc(ReshapeCall);
    glutKeyboardFunc(keyboardCall);
    glutMouseFunc(mouseCall);
    glutTimerFunc(500, timerCall, 1);
    glutMainLoop();
}

char* filetobuf(const string filename)
{
    ifstream inFile(filename, ios::binary);
    vector<char> buf;
    buf.resize(0);
    char temp;
    while (inFile >> noskipws >> temp) {
        buf.push_back(temp);
    }
    buf.push_back(0);
    inFile.close();
    char* resultC = new char[buf.size()];
    for (int i = 0; i < buf.size(); i++)
        resultC[i] = buf[i];
    return resultC;
}

void DrawSceneCall()
{
    glClearColor(backgraoundColor[0], backgraoundColor[1], backgraoundColor[2], 1);
    glClear(GL_COLOR_BUFFER_BIT);
    if (click) {
        glBindVertexArray(VAO);
        if (dotLine) {
            glPointSize(0.5f);
            glDrawArrays(GL_POINTS, 0, dots.size() / 6);
        }
        else {
            glLineWidth(1.2f);
            glDrawArrays(GL_LINE_STRIP, 0, dots.size() / 6);
        }
    }
    glutSwapBuffers();
}

void ReshapeCall(int w, int h)
{
    glViewport(0, 0, w, h);
    Wwidth = w;
    Wheight = h;
}

void keyboardCall(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
        dotLine = true;
        break;
    case 'b':
        dotLine = false;
        break;
    default:
        break;
    }
}

void mouseCall(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT && state == GLUT_DOWN) {
        reversemode = !reversemode;
        random_device rd;
        default_random_engine dre(rd());
        uniform_real_distribution<float> urd(0, 1);
        for (int i = 0; i < 3; i++)
            backgraoundColor[i] = urd(dre);
        dots.resize(0);

        roundConter = 0;
        modeChange = false;
        if (reversemode)
            theta = 0;
        else
            theta = 360;
        radious = 0.06;
        dots.push_back((float)(x - Wwidth / 2) / Wwidth * 2 - 3.5 * radious);
        dots.push_back((float)(Wheight / 2 - y) / Wheight * 2);
        dots.push_back(0);

        dots.push_back(1);
        dots.push_back(0);
        dots.push_back(0);
        start[0] = (float)(x - Wwidth / 2) / Wwidth * 2 - 4.5 * radious;
        start[1] = (float)(Wheight / 2 - y) / Wheight * 2;
        start[2] = 0;
        click = true;
        if (reversemode)
            theta++;
        else
            theta--;
        InitBuffer();
    }
    glutPostRedisplay();
}

void timerCall(int value)
{
    if (click) {
        if (reversemode) {
            if (!modeChange) {
                dots.push_back((float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0]);
                dots.push_back((float)radious * (float)sin((float)(theta * 3.14 / 180)) + start[1]);
                dots.push_back(0);

                dots.push_back(1);
                dots.push_back(0);
                dots.push_back(0);

                if (theta == 180) {
                    start[0] = dots[roundConter * 180 * 6] - radious * 0.5;
                    start[1] = dots[roundConter * 180 * 6 + 1];
                    radious *= 1.5;
                    roundConter++;
                }
                else if (theta == 360) {
                    start[0] = dots[roundConter * 180 * 6] + radious * 0.5;
                    start[1] = dots[roundConter * 180 * 6 + 1];
                    radious *= 1.5;
                    roundConter++;
                }
                theta += 1;
                if (theta == 361)
                    theta = 1;
                if (roundConter == 4) {
                    theta--;
                    start[0] = (float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0] + radious / 1.5;
                    radious /= 1.5;
                    modeChange = true;
                    theta = 179;
                }
            }
            else if (modeChange && roundConter != 8) {
                dots.push_back((float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0]);
                dots.push_back((float)radious * (float)sin((float)(theta * 3.14 / 180)) + start[1]);
                dots.push_back(0);

                dots.push_back(1);
                dots.push_back(0);
                dots.push_back(0);

                if (theta == 180) {
                    start[0] = (float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0] + radious / 1.5;
                    start[1] = ((float)radious * (float)sin((float)(theta * 3.14 / 180)) + start[1]);
                    radious /= 1.5;
                    roundConter++;
                }
                else if (theta == 0) {
                    start[0] = (float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0] - radious / 1.5;
                    start[1] = ((float)radious * (float)sin((float)(theta * 3.14 / 180)) + start[1]);
                    radious /= 1.5;
                    roundConter++;
                }
                theta -= 1;
                if (theta == -1)
                    theta = 359;
            }
        }
        else {
            if (!modeChange) {
                dots.push_back((float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0]);
                dots.push_back((float)radious * (float)sin((float)(theta * 3.14 / 180)) + start[1]);
                dots.push_back(0);

                dots.push_back(1);
                dots.push_back(0);
                dots.push_back(0);

                if (theta == 180) {
                    start[0] = (float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0] + radious * 1.5;
                    start[1] = ((float)radious * (float)sin((float)(theta * 3.14 / 180)) + start[1]);
                    radious *= 1.5;
                    roundConter++;
                }
                else if (theta == 0) {
                    start[0] = (float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0] - radious * 1.5;
                    start[1] = ((float)radious * (float)sin((float)(theta * 3.14 / 180)) + start[1]);
                    radious *= 1.5;
                    roundConter++;
                }
                theta -= 1;
                if (theta == -1)
                    theta = 359;
                if (roundConter == 4) {
                    theta++;
                    start[0] = (float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0] + radious / 1.5;
                    radious /= 1.5;
                    modeChange = true;
                    theta = 181;
                }
            }
            else if (modeChange && roundConter != 8) {
                dots.push_back((float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0]);
                dots.push_back((float)radious * (float)sin((float)(theta * 3.14 / 180)) + start[1]);
                dots.push_back(0);

                dots.push_back(1);
                dots.push_back(0);
                dots.push_back(0);

                if (theta == 180) {
                    start[0] = (float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0] + radious / 1.5;
                    start[1] = dots[roundConter * 180 * 6 + 1];
                    radious /= 1.5;
                    roundConter++;
                }
                else if (theta == 360) {
                    start[0] = (float)radious * (float)cos((float)(theta * 3.14 / 180)) + start[0] - radious / 1.5;
                    start[1] = dots[roundConter * 180 * 6 + 1];
                    radious /= 1.5;
                    roundConter++;
                }
                theta += 1;
                if (theta == 361)
                    theta = 1;
            }
        }
    }
    InitBuffer();
    glutPostRedisplay();
    glutTimerFunc(1, timerCall, value);
}

void makeVertexShader()
{
    vertexSource = filetobuf("vertex.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        cerr << "VERTEXSHADER ERROR: " << errorLog << endl;
    }
}

void makeFragmentShader()
{
    fragmentSource = filetobuf("fragment.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        cerr << "FRAGMENT SHADER ERROR: " << errorLog << endl;
    }
}

void makeShaderID()
{
    makeVertexShader();
    makeFragmentShader();

    ShaderID = glCreateProgram();

    glAttachShader(ShaderID, vertexShader);
    glAttachShader(ShaderID, fragmentShader);

    glLinkProgram(ShaderID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint result;
    glGetProgramiv(ShaderID, GL_LINK_STATUS, &result);
    GLchar errorLog[512];
    if (!result) {
        glGetProgramInfoLog(ShaderID, 512, NULL, errorLog);
        cerr << "ShaderID Program ERROR: " << errorLog << endl;
    }
}

void InitBuffer()
{
    if (click) {
        /*int a = 0;
        for (auto k = dots.begin(); k < dots.end(); k++) {
           if(a%6 == 0)
              cout << *k << " ";
           a++;
        }*/
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(2, VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * dots.size(), &dots[0], GL_STREAM_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * dots.size(), &dots[0], GL_STREAM_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

    }
}

//int a(int k) {
//
//
//}