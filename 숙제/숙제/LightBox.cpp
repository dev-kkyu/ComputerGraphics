#include "LightBox.h"

LightBox::LightBox() : Unit(glm::mat4(1.f))
{
}

LightBox::LightBox(glm::vec3 Position)
	: Unit(glm::mat4(1.f)), Position(Position), LightAngle(0.f), Color(glm::vec3(0.7))
{
	Update(0);
}

void LightBox::setAngle(float Angle)
{
	LightAngle = Angle;
}

void LightBox::Update(int Direction)
{
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(2));
	glm::mat4 Trans = glm::translate(Unit, Position);
	Rotate = glm::rotate(Unit, glm::radians(LightAngle), glm::vec3(0, 1, 0));

	Change = Rotate * Trans * Scale;

	LightAngle += (float)Direction;
}

void LightBox::Draw()
{
	glBindVertexArray(VAO);

	glm::vec4 lightPos(Position.x - 2.5f, Position.y, Position.z, 1);
	lightPos = Rotate * lightPos;

	unsigned int lightPosLocation = glGetUniformLocation(shaderID, "lightPos");		//--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);

	GLuint Color = glGetUniformLocation(shaderID, "objectColor");
	glUniform3f(Color, this->Color.r, this->Color.g, this->Color.b);

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

