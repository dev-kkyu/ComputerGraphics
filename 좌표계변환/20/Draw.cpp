#include "Draw.h"

GLfloat openF = 0.f;

Robot robot;



GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	const glm::mat4 Unit(1.0f);

	if (isDepTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//-----------------------------------------------------�𵨺�ȯ-------------------------------------------------------
	//-----------------------------------------------------���ڱ׸���-----------------------------------------------------
	glm::mat4 Trans;
	glm::mat4 Rotate;
	glm::mat4 Scale;
	glm::mat4 Change;

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f))); //--- modelTransform ������ ��ȯ �� �����ϱ�

	GLuint option = glGetUniformLocation(shaderID, "option");
	GLuint Color = glGetUniformLocation(shaderID, "Color");

	glUniform1i(option, 1);													// ���� ������ ����� �ٲܰ���
	//glUniform1i(option, 0);													// ���� �ִ� ���̴����ۿ� �ִ»���� ������

	glUniform3f(Color, 0.635f, 1.f, 1.f);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 3 * 2)));		//�޸�

	glUniform3f(Color, 1.f, 1.f, 0.5f);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)(sizeof(float) * (1 * 3 * 2)));		//����

	glUniform3f(Color, 0.81f, 1.f, 0.14f);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)(sizeof(float) * (2 * 3 * 2)));		//����

	glUniform3f(Color, 1.f, 0.8f, 0.8f);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)(sizeof(float) * (3 * 3 * 2)));		//������

	glUniform3f(Color, 1.f, 1.f, 1.f);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)(sizeof(float) * (4 * 3 * 2)));		//�Ʒ���



	Trans = glm::translate(Unit, glm::vec3(0.f, openF, 0.f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Trans)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glUniform3f(Color, 0.8f, 0.7f, 1.f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(float) * (5 * 3 * 2)));		//����


	//------------------------------------------------------�κ��׸���---------------------------------------------------------

	robot.draw();

	//------------------------------------------------------��ֹ��׸���---------------------------------------------------------







	//for (int i = 0; i < face[diag1].size(); ++i)
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));

	glutSwapBuffers();							// ȭ�鿡 ����ϱ�
}