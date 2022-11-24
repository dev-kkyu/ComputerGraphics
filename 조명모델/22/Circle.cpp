#include "Circle.h"

Circle::Circle()
{
	Color = glm::vec3(1.f, 1.f, 1.f);
	qobj = gluNewQuadric();
}

void Circle::FixUpdate()
{
	glm::mat4 Rotate = glm::rotate(Unit, glm::radians(90.f), glm::vec3(1, 0, 0));
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(4.f + L_MovValue, 1.f, 4.f + L_MovValue));
	Change = Scale * Rotate;
}

void Circle::Draw()
{
	GLuint objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, Color.r, Color.g, Color.b);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	gluCylinder(qobj, 1.0, 1.0, 0.015, 100, 8);
}
