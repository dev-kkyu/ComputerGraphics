#include "Draw.h"
#include "SierpinskiPyramid.h"
#include "Light.h"
#include "Sphere.h"
#include "Snow.h"

Sphere SPHERE[3]{
	{Sphere(2.f, 0.5f, 1.f, glm::vec3{1,0,0}, 30.f, 0.f)},
	{Sphere(2.2f, 0.4f, 1.2f, glm::vec3{0,1,0}, 60.f, 20.f)},
	{Sphere(2.5f, 0.3f, 0.8f, glm::vec3{0,0,1}, 75.f, 30.f)}
};

Snow SNOW[100];

SierpinskiPyramid Pry(glm::vec3(0, 1, 1));

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
	//glEnable(GL_CULL_FACE);		//�ø� �������ֱ�

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// ��������
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 50.f);
	GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");	//--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	// ī�޶�
	glm::vec3 cameraPos = glm::vec3(0,5,5);		//--- ī�޶� ��ġ (��� ������)
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);					//--- ī�޶� �ٶ󺸴� ���� (��𺼰��� �ϸ�ɵ�)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);							//--- ī�޶� ���� ����->������(���⸸) (�����ϸ� ȭ�� ���ϰŲٷκ���)

	glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	unsigned int viewPosLocation = glGetUniformLocation(shaderID, "viewPos");		//--- viewPos �� ����: ī�޶� ��ġ
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);


	glBindVertexArray(floorVAO);

	unsigned int objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	glUniform3f(objColorLocation, 0.8,0.8,0.8);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.f), glm::vec3(2, 0, 2))));

	glDrawArrays(GL_QUADS, 0, 4);

	
	LIGHT.Draw();
	for(Sphere& sph: SPHERE)
		sph.Draw();
	Pry.Draw(FractalNum);
	for (Snow& snow : SNOW) {
		snow.Draw();
	}

	glutSwapBuffers();							// ȭ�鿡 ����ϱ�
}
