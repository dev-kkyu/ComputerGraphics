#include "Cube.h"

Cube::Cube()
{
	Color = glm::vec3(0, 0, 1);
}

void Cube::FixUpdate()
{
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(1, 1, 1));
	Change = Scale;

	Rotate = glm::rotate(Unit, glm::radians(O_RotYAngle), glm::vec3(0, 1, 0));
	Rotate = glm::rotate(Rotate, glm::radians(O_RotXAngle), glm::vec3(1, 0, 0));
}

void Cube::Draw()
{
	glBindVertexArray(cubeVAO);

	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");	//--- 텍스처 사용
	glUniform1i(selectColorLocation, 1);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	for (int i = 0; i < 6; ++i) {
		glBindTexture(GL_TEXTURE_2D, skTexture[i]);
		glDrawArrays(GL_TRIANGLES, i * 6, 6);
	}
}
