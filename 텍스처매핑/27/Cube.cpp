#include "Cube.h"

Cube::Cube(glm::vec3 Position) : Unit{ glm::mat4(1.f) }, Position{ Position }
{
	Color = glm::vec3(0, 0.5, 1);
}

void Cube::Update()
{
	glm::mat4 Trans;
	glm::mat4 Scale;

	Trans = glm::translate(Unit, glm::vec3(0, 1, 0));
	Scale = glm::scale(Unit, glm::vec3(0.5, 1.5, 0.5));

	Change = Scale * Trans;

	Trans = glm::translate(Unit, Position);

	Change = Trans * Change;
}

void Cube::Draw()
{
	glBindVertexArray(cubeVAO);

	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");
	glUniform1i(selectColorLocation, 0);			// 색상모드

	GLuint objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform4f(objColorLocation, Color.r, Color.g, Color.b, 0.5f);

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change));

	glDrawArrays(GL_TRIANGLES, 0, cubevertex.size() * 3);
}