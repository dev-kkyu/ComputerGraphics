#include "Draw.h"

float O_RotYAngle;
float O_RotXAngle;
float L_RotYAngle;
float L_MovValue;

Cube CUBE;
Pyramid PYRAMID;
Light LIGHT;
Circle CIRCLE;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	if (isDepTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	//glFrontFace(GL_CW);			//시계방향이 앞면으로 설정(박스 바깥쪽이 뒷면, 안쪽이 앞면)
	//glEnable(GL_CULL_FACE);		//컬링 설정해주기
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDisable(GL_BLEND);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");
	GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");	//--- 투영 변환 값 설정
	GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- 뷰잉 변환 설정
	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");


	// 뒷면 그리기
	glBindVertexArray(floorVAO);
	glBindTexture(GL_TEXTURE_2D, floorTexture);

	glUniform1i(selectColorLocation, 1);			// 텍스처모드

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -20))
		* glm::scale(glm::mat4(1.f), glm::vec3(8.5, 8.5, 1))));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));

	glDrawArrays(GL_QUADS, 0, 4);



	// 원근투영
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 50.f);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	// 카메라
	//glm::vec3 cameraPos = glm::vec3(-7,4,15);		//--- 카메라 위치 (어디서 볼건지)
	glm::vec3 cameraPos = glm::vec3(-4, 4, 7);		//--- 카메라 위치 (어디서 볼건지)
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);					//--- 카메라 바라보는 방향 (어디볼건지 하면될듯)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);							//--- 카메라 위쪽 방향->벡터임(방향만) (음수하면 화면 상하거꾸로보임)

	glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::vec4 lightPos(0, 0, 3.4 + L_MovValue, 1);
	glm::mat4 RotL = glm::rotate(glm::mat4(1.f), glm::radians(L_RotYAngle), glm::vec3(0, 1, 0));
	lightPos = RotL * lightPos;

	unsigned int lightPosLocation = glGetUniformLocation(shaderID, "lightPos");		//--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	unsigned int lightColorLocation = glGetUniformLocation(shaderID, "lightColor");	//--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	unsigned int viewPosLocation = glGetUniformLocation(shaderID, "viewPos");		//--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	//선 그리기
	glUniform1i(selectColorLocation, 0);			// 텍스처모드
	unsigned int objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1, 1, 1);
	glm::mat4 Rotate{ glm::rotate(glm::mat4(1.f), glm::radians(O_RotYAngle), glm::vec3(0, 1, 0)) };
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Rotate));
	glBindVertexArray(crossVAO);
	glLineWidth(2.f);
	glDrawArrays(GL_LINES, 0, 6);
	glLineWidth(1.f);

	if (isRect)
		CUBE.Draw();
	else
		PYRAMID.Draw();
	LIGHT.Draw();
	CIRCLE.Draw();







	glutSwapBuffers();							// 화면에 출력하기
}
