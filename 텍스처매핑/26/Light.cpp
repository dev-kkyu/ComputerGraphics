#include "Light.h"

Light::Light()
{
	Color = glm::vec3(0.8, 0.8, 0.8);
}

void Light::FixUpdate()
{
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.75f, 0.5f, 0.5f));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(0, 0, 4 + L_MovValue));
	Change = Trans * Scale;

	Rotate = glm::rotate(Unit, glm::radians(L_RotYAngle), glm::vec3(0, 1, 0));
}

void Light::Draw()
{
	glBindVertexArray(cubeVAO);

	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");	//--- 텍스처 안쓰고 색상 사용
	glUniform1i(selectColorLocation, 0);

	GLuint objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, Color.r, Color.g, Color.b);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	glDrawArrays(GL_TRIANGLES, 0, cubevertex.size() * 3);
}
