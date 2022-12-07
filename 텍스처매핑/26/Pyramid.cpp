#include "Pyramid.h"

Pyramid::Pyramid()
{
	Color = glm::vec3(0, 1, 0);
}

void Pyramid::FixUpdate()
{
	Change = Unit;

	Rotate = glm::rotate(Unit, glm::radians(O_RotYAngle), glm::vec3(0, 1, 0));
	Rotate = glm::rotate(Rotate, glm::radians(O_RotXAngle), glm::vec3(1, 0, 0));
}

void Pyramid::Draw()
{
	glBindVertexArray(pyramidVAO);

	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");	//--- 텍스처 사용
	glUniform1i(selectColorLocation, 1);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));


	for (int i = 0; i < 4; ++i) {
		glBindTexture(GL_TEXTURE_2D, skTexture[i]);
		glDrawArrays(GL_TRIANGLES, i * 3, 3);
	}
	glBindTexture(GL_TEXTURE_2D, skTexture[4]);
	glDrawArrays(GL_TRIANGLES, 4 * 3, 6);
}
