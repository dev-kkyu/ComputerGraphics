#include "Pyramid.h"

Pyramid::Pyramid()
{
	Color = glm::vec3(0, 1, 0);
}

void Pyramid::FixUpdate()
{
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(0, 1, 0));
	Change = Trans;

	Rotate = glm::rotate(Unit, glm::radians(O_RotYAngle), glm::vec3(0, 1, 0));
}

void Pyramid::Draw()
{
	glBindVertexArray(pyramidVAO);

	unsigned int objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, Color.r, Color.g, Color.b);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	glDrawArrays(GL_TRIANGLES, 0, pyramidvertex.size() * 3);
}
