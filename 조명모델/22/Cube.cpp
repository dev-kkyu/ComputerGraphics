#include "Cube.h"

Cube::Cube()
{
	Color = glm::vec3(0, 0, 1);
}

void Cube::FixUpdate()
{
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(1.5f, 1, 1));
	Change = Scale;

	Rotate = glm::rotate(Unit, glm::radians(O_RotYAngle), glm::vec3(0, 1, 0));
}

void Cube::Draw()
{
	glBindVertexArray(cubeVAO);

	unsigned int objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, Color.r, Color.g, Color.b);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	glDrawArrays(GL_TRIANGLES, 0, cubevertex.size() * 3);
}
