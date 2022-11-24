#include "Light.h"

Light::Light() : Unit(glm::mat4(1.f)), Change(glm::mat4(1.f)), Color(glm::vec3(0.8, 0.8, 0.8))
{
}

void Light::Update()
{
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.3f, 0.3f, 0.3f));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(8, 0, 0));
	glm::mat4 Rotate = glm::rotate(Unit, glm::radians(L_RotYAngle), glm::vec3(0, 1, 0));
	
	Change = Rotate * Trans * Scale;
}

void Light::Draw()
{
	glBindVertexArray(cubeVAO);

	unsigned int objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, Color.r, Color.g, Color.b);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	glDrawArrays(GL_TRIANGLES, 0, cubevertex.size() * 3);
}
