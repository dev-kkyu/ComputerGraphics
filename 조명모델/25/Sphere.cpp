#include "Sphere.h"

Sphere::Sphere(float CircleScale, float sphScale, float Speed, glm::vec3 Color, float rotY, float rotZ)
	: Unit{ glm::mat4(1.f) },
	CircleScale{ CircleScale }, sphScale{ sphScale }, Speed{ Speed }, Color{ Color }, nowZAngle{ 0.f },
	rotY{ rotY }, rotZ{ rotZ }
{
	qobj = gluNewQuadric();

	CircleAngle = glm::rotate(Unit, glm::radians(rotZ), glm::vec3(0, 0, 1)) *
		glm::rotate(Unit, glm::radians(rotY), glm::vec3(0, 1, 0));

	CircleChange = CircleAngle * glm::scale(Unit, glm::vec3(CircleScale, CircleScale, 1));
}

void Sphere::Update()
{
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(sphScale));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(CircleScale, 0, 0));
	glm::mat4 Rotate = glm::rotate(Unit, glm::radians(++nowZAngle * Speed), glm::vec3(0, 0, 1));

	Change = CircleAngle * Rotate * Trans * Scale;
}

void Sphere::Draw()
{
	GLuint objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, Color.r, Color.g, Color.b);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(CircleChange));
	gluCylinder(qobj, 1.0, 1.0, 0.015, 100, 8);


	glBindVertexArray(sphereVAO);

	model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	glDrawArrays(GL_TRIANGLES, 0, spherevertex.size() * 3);
}
