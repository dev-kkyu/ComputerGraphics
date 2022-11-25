#include "Draw.h"

float C_movX, C_movZ;
float C_RotYAngle;

bool isMakeMountain;

World newWorld;

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	if (isDepTest)
		glEnable(GL_DEPTH_TEST);		//��������
	else
		glDisable(GL_DEPTH_TEST);

	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);

	glEnable(GL_DITHER);        // ǥ���� �Ų�����
	glEnable(GL_CULL_FACE);     // �ø�
	glEnable(GL_LINE_SMOOTH);   // ��Ƽ �ٸ����
	glEnable(GL_POLYGON_SMOOTH);// ��Ƽ �ٸ����
	glShadeModel(GL_SMOOTH);    // �ε巯�� ������ �����մϴ�.

	//glFrontFace(GL_CW);			//�ð������ �ո����� ����(�ڽ� �ٱ����� �޸�, ������ �ո�)
	//glEnable(GL_CULL_FACE);		//�ø� �������ֱ�

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	unsigned int lightColorLocation = glGetUniformLocation(shaderID, "lightColor");	//--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	glUniform3f(lightColorLocation, 245 / 255., 214 / 255., 1.f);

	glViewport(0, 0, winWidth, winHeight);
	normalView();
	newWorld.Draw();



	glViewport(winWidth / 4 * 3 - 10, winHeight / 4 * 3 - 10, winWidth / 4, winHeight / 4);
	//�⺻ �� �׵α� �׸���
	resetView();
	drawLine();

	topView();
	newWorld.Draw();


	glutSwapBuffers();							// ȭ�鿡 ����ϱ�

}

void resetView()
{
	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));

	unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));

	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
}

void drawLine()
{
	GLuint Color = glGetUniformLocation(shaderID, "objectColor");

	glBindVertexArray(rectVAO);
	glLineWidth(4.f);
	glUniform3f(Color, 1.f, 1.f, 1.f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glLineWidth(1.f);
}

void normalView()
{	
	// ��������

	//ī�޶�
	newWorld.Camera(personView);
}

void topView()
{
	// ��������
	glm::mat4 projection = glm::ortho(-30.0f * ((float)winWidth / (float)winHeight), 30.0f * ((float)winWidth / (float)winHeight), -30.0f, 30.0f, -30.0f, 30.0f);
	GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	// ī�޶�
	glm::vec3 cameraPos = glm::vec3(C_movX, 10.f + C_movZ, 0.f); //--- ī�޶� ��ġ (��� ������)
	glm::vec3 cameraDirection = glm::vec3(C_movX, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ���� (��𺼰��� �ϸ�ɵ�)
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, -1.0f); //--- ī�޶� ���� ����->������(���⸸) (�����ϸ� ȭ�� ���ϰŲٷκ���)

	glm::mat4 cameraRotationY = glm::rotate(glm::mat4(1.f), glm::radians(MouseAngle.first), glm::vec3(0.f, 0.f, 1.f));

	glm::mat4 view;
	if (personView == 1 || personView == 3)
		view = cameraRotationY * glm::lookAt(cameraPos, cameraDirection, cameraUp);
	else
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
}
