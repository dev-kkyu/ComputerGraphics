#include "RobotParts.h"

RobotParts::RobotParts()
	: Unit(glm::mat4(1.f)), Change(glm::mat4(1.f)), Color(glm::vec3(1.f)), Position(glm::vec3(0.f, -1.f, 0.f)), Direction(0.f)
{
}

RobotParts::~RobotParts()
{
}

void RobotParts::draw()
{
	GLuint Color = glGetUniformLocation(shaderID, "objectColor");

	glUniform3f(Color, this->Color.r, this->Color.g, this->Color.b);

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void RobotParts::keyIn(glm::vec3 Position, float Direction)
{
	this->Position = Position;
	this->Direction = Direction;
	Update();
}
