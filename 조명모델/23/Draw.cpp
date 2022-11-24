#include "Draw.h"

float L_RotYAngle;

Light LIGHT;
Sphere SPHERE[3]{ Sphere(0., glm::vec3(1, 0, 0), 1),Sphere(-4., glm::vec3(0, 1, 0), 0.8),Sphere(-7., glm::vec3(0, 0, 1), 0.6) };


GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	if (isDepTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	//glFrontFace(GL_CW);			//�ð������ �ո����� ����(�ڽ� �ٱ����� �޸�, ������ �ո�)
	//glEnable(GL_CULL_FACE);		//�ø� �������ֱ�

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// ��������
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 50.f);
	GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");	//--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	// ī�޶�
	glm::vec3 cameraPos = glm::vec3(0,0,16);		//--- ī�޶� ��ġ (��� ������)
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);					//--- ī�޶� �ٶ󺸴� ���� (��𺼰��� �ϸ�ɵ�)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);							//--- ī�޶� ���� ����->������(���⸸) (�����ϸ� ȭ�� ���ϰŲٷκ���)

	glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::vec4 lightPos(7.5, 0, 0 , 1);
	glm::mat4 RotL = glm::rotate(glm::mat4(1.f), glm::radians(L_RotYAngle), glm::vec3(0, 1, 0));
	lightPos = RotL * lightPos;

	unsigned int lightPosLocation = glGetUniformLocation(shaderID, "lightPos");		//--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	unsigned int lightColorLocation = glGetUniformLocation(shaderID, "lightColor");	//--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	
	switch (colorValue) {
	case 0:
		glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
		break;
	case 1:
		glUniform3f(lightColorLocation, 1.0, 0.0, 0.0);
		break;
	case 2:
		glUniform3f(lightColorLocation, 0.0, 1.0, 0.0);
		break;
	case 3:
		glUniform3f(lightColorLocation, 0.0, 0.0, 1.0);
		break;
	}

	unsigned int viewPosLocation = glGetUniformLocation(shaderID, "viewPos");		//--- viewPos �� ����: ī�޶� ��ġ
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	LIGHT.Draw();
	for (Sphere& sph : SPHERE)
		sph.Draw();


	glutSwapBuffers();							// ȭ�鿡 ����ϱ�
}
