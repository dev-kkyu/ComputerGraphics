#include "Draw.h"
#include "SierpinskiPyramid.h"
#include "Light.h"
#include "Snow.h"
#include "Cube.h"

float cameraRotAngle;

Snow SNOW[500];
Cube CUBE[12]{
	Cube(glm::vec3(-6, 0, -6)),
	Cube(glm::vec3(-2, 0, -6)),
	Cube(glm::vec3(2, 0, -6)),
	Cube(glm::vec3(6, 0, -6)),
	Cube(glm::vec3(-6, 0, 6)),
	Cube(glm::vec3(-2, 0, 6)),
	Cube(glm::vec3(2, 0, 6)),
	Cube(glm::vec3(6, 0, 6)),

	Cube(glm::vec3(-6, 0, -2)),
	Cube(glm::vec3(-6, 0, 2)),
	Cube(glm::vec3(6, 0, -2)),
	Cube(glm::vec3(6, 0, 2))
};


SierpinskiPyramid Pry(glm::vec3(0, 1, 0.5));

float L_RotYAngle;
int FractalNum;

Light LIGHT;

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	if (isDepTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	//glFrontFace(GL_CW);			//�ð������ �ո����� ����(�ڽ� �ٱ����� �޸�, ������ �ո�)
	glEnable(GL_CULL_FACE);		//�ø� �������ֱ�

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// ��������
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)winWidth / (float)winHeight, 0.1f, 50.f);
	GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");	//--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	// ī�޶�
	glm::vec3 cameraPos = glm::vec3(0, 3, 15);		//--- ī�޶� ��ġ (��� ������)
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);					//--- ī�޶� �ٶ󺸴� ���� (��𺼰��� �ϸ�ɵ�)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);							//--- ī�޶� ���� ����->������(���⸸) (�����ϸ� ȭ�� ���ϰŲٷκ���)

	glm::mat4 Rotate = glm::rotate(glm::mat4(1.f), glm::radians(cameraRotAngle), glm::vec3(0, 1, 0));
	cameraPos = Rotate * glm::vec4(cameraPos, 1.f);

	glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	GLuint viewPosLocation = glGetUniformLocation(shaderID, "viewPos");		//--- viewPos �� ����: ī�޶� ��ġ
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);


	glBindVertexArray(floorVAO);
	glBindTexture(GL_TEXTURE_2D, floorTexture);

	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");
	glUniform1i(selectColorLocation, 1);			// �ؽ�ó���

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.f), glm::vec3(8, 0, 8))));

	glDrawArrays(GL_QUADS, 0, 4);

	// �������Ѱ� ����
	LIGHT.Draw();
	Pry.Draw(FractalNum);

	if (isDrowSnow)
		for (Snow& snow : SNOW)
			snow.Draw();

	// �״��� �����Ѱ�
	for (Cube& cube : CUBE)
		cube.Draw();

	glutSwapBuffers();							// ȭ�鿡 ����ϱ�
}
