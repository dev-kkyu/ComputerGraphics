#include "LightBox.h"

LightBox::LightBox() : Unit(glm::mat4(1.f)), Change(glm::mat4(1.f))
{
}

void LightBox::Update()
{
	glm::mat4 Rotate(glm::rotate(Unit, glm::radians(L_RotYAngle), glm::vec3(0, 1, 0)));
	glm::mat4 revRotate(glm::rotate(Unit, glm::radians(L_RotYAngle), glm::vec3(0, -1, 0)));
	glm::mat4 Trans(glm::translate(Unit, glm::vec3(2, 1, 0)));
	glm::mat4 Scale(glm::scale(Unit, glm::vec3(0.2,0.2,0.01)));

	Change = Rotate * Trans * revRotate * Scale;
}

void LightBox::Draw()
{
	glBindVertexArray(pentaVAO);

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");

	GLuint	SelectColor = glGetUniformLocation(shaderID, "SelectColor");
	glUniform1i(SelectColor, 1);

	GLuint objectColor = glGetUniformLocation(shaderID, "objectColor");
	glUniform3f(objectColor, 1, 1, 1);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기

	glDrawArrays(GL_POLYGON, 0, 5);
}
