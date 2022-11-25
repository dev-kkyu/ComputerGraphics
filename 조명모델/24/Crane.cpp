#include "Crane.h"

Crane::Crane() : Unit(glm::mat4(1.f)), Change(glm::mat4(1.f))
{
}

void Crane::Draw()
{
	glBindVertexArray(rectVAO);
	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");

	GLuint	SelectColor = glGetUniformLocation(shaderID, "SelectColor");
	glUniform1i(SelectColor, 1);

	GLuint objectColor = glGetUniformLocation(shaderID, "objectColor");

	BottomUpdate();
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3f(objectColor, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	MiddleUpdate();
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3f(objectColor, 0, 1, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	TopUpdate(-1);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3f(objectColor, 0, 0, 1);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	TopUpdate(1);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3f(objectColor, 0, 0, 1);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Crane::BottomUpdate()
{
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.25, 0.1, 0.25));

	Change = Scale * Trans;
}

void Crane::MiddleUpdate()
{
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(0.0, 5.0, 0.0));			//���� ���� ������ 2�� �����̸� �� ����, ������ ���� ������ ������ �ѱ���ġ��ŭ ����?
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.125, 0.05, 0.125));

	Change = Scale * Trans;
}

void Crane::TopUpdate(int i)
{
	glm::mat4 TransArm = glm::translate(Unit, glm::vec3(0.05 * i, 0.0, 0.0));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(0.0, 0.4, 0.0));			// ���� �ø��°�
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.04, 0.1, 0.04));

	Change = TransArm * Trans * Scale;
}
