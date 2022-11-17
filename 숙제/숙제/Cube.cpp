#include "Cube.h"

Cube::Cube() : Unit(glm::mat4(1.f))
{
}

Cube::Cube(glm::vec3 Position, float Xscale, float Zscale)
	: Unit(glm::mat4(1.f)), Position(Position), Xscale(Xscale), Zscale(Zscale), isRender(true)		//Position의 y는 0으로 고정한다.
{
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> Curd{ 0.f, 1.f };
	Color = glm::vec3(Curd(dre), Curd(dre), Curd(dre));

	uniform_real_distribution<float> maxSurd{ 5.f, 15.f };
	maxScale = maxSurd(dre);

	uniform_real_distribution<float> Surd{ 0.f, maxScale };
	Yscale = Surd(dre);

	uniform_int_distribution<int> Durd{ 0, 1 };
	Direction = Durd(dre);
	if (!Direction)
		Direction = -1;		//-1 또는 1로 만듦

	uniform_real_distribution<float> speedUrd{ 0.05f, 0.1f };
	speed = speedUrd(dre);

	Update();

}

Cube::~Cube()
{
}

void Cube::FixUpdate()
{

}

void Cube::Update()
{
	if (isUpDownMove) {
		glm::mat4 Scale;
		glm::mat4 Trans;
		glm::mat4 Rotate;

		if (Yscale <= 0.f)
			Direction = 1;
		else if (Yscale >= maxScale)
			Direction = -1;

		Yscale += speed * Direction * MoveSpeed;


		Trans = glm::translate(Unit, glm::vec3(0, 1, 0));
		Scale = glm::scale(Unit, glm::vec3(Xscale, Yscale, Zscale));

		Change = Scale * Trans;

		Trans = glm::translate(Unit, Position);

		Change = Trans * Change;
	}
	else if (isMoveStopNDown) {
		glm::mat4 Scale;
		glm::mat4 Trans;
		glm::mat4 Rotate;

		if (maxScale / 3 < Yscale)
			Yscale -= speed;


		Trans = glm::translate(Unit, glm::vec3(0, 1, 0));
		Scale = glm::scale(Unit, glm::vec3(Xscale, Yscale, Zscale));

		Change = Scale * Trans;

		Trans = glm::translate(Unit, Position);

		Change = Trans * Change;
	}
}

void Cube::Draw()
{
	if (isRender) {
		glBindVertexArray(VAO);

		GLuint Color = glGetUniformLocation(shaderID, "Color");
		glUniform3f(Color, this->Color.r, this->Color.g, this->Color.b);

		GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환

		glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 0)));
	}
}

void Cube::SetRender(bool isRender)
{
	this->isRender = isRender;
}

bool Cube::GetRender()
{
	return isRender;
}

float Cube::getLeft()
{
	return Position.x - Xscale;
}

float Cube::getRight()
{
	return Position.x + Xscale;
}

float Cube::getBehind()
{
	return Position.z - Zscale;
}

float Cube::getFront()
{
	return Position.z + Zscale;
}

float Cube::getBottom()
{
	return 0.0f;
}

float Cube::getTop()
{
	return 2.0f;
}
