#include "Sphere.h"

Sphere::Sphere()
	: Object()
{
}

Sphere::Sphere(float scale)
	: Object(scale)
{
	qobj = gluNewQuadric(); // 객체 생성하기

	random_device rd;
	default_random_engine dre(rd());

	uniform_real_distribution<float> uidC{ 0.f, 0.5f };
	Color = glm::vec3(uidC(dre), uidC(dre), uidC(dre));

	uniform_real_distribution<float> uidP{ -1.f + scale, 1.f - scale };
	Position = glm::vec3(uidP(dre), uidP(dre), uidP(dre));

	uniform_int_distribution<int> uidD{ 0, 1 };
	for (int i = 0; i < 3; ++i) {
		int temp = uidD(dre);
		if (temp)
			Direction[i] = 1;
		else
			Direction[i] = -1;
	}

	Update();
}

Sphere::~Sphere()
{
}

void Sphere::FixUpdate()
{
	if (Position.x + scale >= 1.f|| Position.x - scale <= -1.f)
		Direction.x *= -1;
	if (Position.y + scale >= 1.f || Position.y - scale <= -1.f)
		Direction.y *= -1;
	if (Position.z + scale >= 1.f || Position.z - scale <= -1.f)
		Direction.z *= -1;
}

void Sphere::Update()
{
	FixUpdate();

	Position.x += 0.01 * Direction.x;
	Position.y += 0.01 * Direction.y;
	Position.z += 0.01 * Direction.z;

	glm::mat4 Scale = glm::scale(Unit, glm::vec3(scale));
	glm::mat4 Trans = glm::translate(Unit, Position);
	glm::mat4 Rotate = glm::rotate(Unit, glm::radians((float)CubeAngle), glm::vec3(0.f, 0.f, 1.f));

	Change = Rotate * Trans * Scale;
}

void Sphere::Draw()
{
	GLuint Color = glGetUniformLocation(shaderID, "Color");
	glUniform3f(Color, this->Color.r, this->Color.g, this->Color.b);

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환

	gluQuadricDrawStyle(qobj, GLU_FILL); // 도형 스타일
	gluSphere(qobj, 1., 50, 50); // 객체 만들기
}
