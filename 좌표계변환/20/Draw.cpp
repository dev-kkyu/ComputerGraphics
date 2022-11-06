#include "Draw.h"

GLfloat openF = 0.f;

Robot robot;



GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	const glm::mat4 Unit(1.0f);

	if (isDepTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//-----------------------------------------------------모델변환-------------------------------------------------------
	//-----------------------------------------------------상자그리기-----------------------------------------------------
	glm::mat4 Trans;
	glm::mat4 Rotate;
	glm::mat4 Scale;
	glm::mat4 Change;

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f))); //--- modelTransform 변수에 변환 값 적용하기

	GLuint option = glGetUniformLocation(shaderID, "option");
	GLuint Color = glGetUniformLocation(shaderID, "Color");

	glUniform1i(option, 1);													// 내가 지정한 색깔로 바꿀거임
	//glUniform1i(option, 0);													// 원래 있던 쉐이더버퍼에 있는색깔로 쓸거임

	glUniform3f(Color, 0.635f, 1.f, 1.f);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 3 * 2)));		//뒷면

	glUniform3f(Color, 1.f, 1.f, 0.5f);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)(sizeof(float) * (1 * 3 * 2)));		//왼쪽

	glUniform3f(Color, 0.81f, 1.f, 0.14f);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)(sizeof(float) * (2 * 3 * 2)));		//위쪽

	glUniform3f(Color, 1.f, 0.8f, 0.8f);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)(sizeof(float) * (3 * 3 * 2)));		//오른쪽

	glUniform3f(Color, 1.f, 1.f, 1.f);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)(sizeof(float) * (4 * 3 * 2)));		//아래쪽



	Trans = glm::translate(Unit, glm::vec3(0.f, openF, 0.f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Trans)); //--- modelTransform 변수에 변환 값 적용하기

	glUniform3f(Color, 0.8f, 0.7f, 1.f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(float) * (5 * 3 * 2)));		//앞쪽


	//------------------------------------------------------로봇그리기---------------------------------------------------------

	robot.draw();

	//------------------------------------------------------장애물그리기---------------------------------------------------------







	//for (int i = 0; i < face[diag1].size(); ++i)
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));

	glutSwapBuffers();							// 화면에 출력하기
}