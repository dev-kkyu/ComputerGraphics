#include "Sphere.h"

Sphere::Sphere(float XPostion, glm::vec3 Color, float Scale) : XPostion(XPostion), Unit(glm::mat4(1.f)), Change(glm::mat4(1.f)), Color(Color), scale(glm::vec3(Scale, Scale, Scale))
{
}

void Sphere::Update()
{
	glm::mat4 Scale = glm::scale(Unit, scale);
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(XPostion, 0, 0));

	Change = Trans * Scale;
}

void Sphere::Draw()
{
	glBindVertexArray(sphereVAO);

	unsigned int objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, Color.r, Color.g, Color.b);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	glDrawArrays(GL_TRIANGLES, 0, spherevertex.size() * 3);
}
