#include "Draw.h"

float L_RotYAngle;

float C_movX, C_movZ;
float C_RotYAngle;

Crane CRANE;
LightBox LIGHT;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	if (isDepTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	//glFrontFace(GL_CW);			//시계방향이 앞면으로 설정(박스 바깥쪽이 뒷면, 안쪽이 앞면)
	//glEnable(GL_CULL_FACE);		//컬링 설정해주기

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// 원근투영
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 50.f);
	GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");	//--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	// 카메라

	glm::vec3 CPos(C_movX, 1, 4 + C_movZ);
	glm::mat4 RotC = glm::rotate(glm::mat4(1.f), glm::radians(C_RotYAngle), glm::vec3(0, 1, 0));
	CPos = RotC * glm::vec4(CPos, 1.f);

	glm::vec3 CDir(C_movX + 0.0f, 1.0f, 0.0f);
	CDir = RotC * glm::vec4(CDir, 1.f);

	glm::vec3 cameraPos = glm::vec3(CPos.x, CPos.y, CPos.z);		//--- 카메라 위치 (어디서 볼건지)
	glm::vec3 cameraDirection = glm::vec3(CDir.x, CDir.y, CDir.z);					//--- 카메라 바라보는 방향 (어디볼건지 하면될듯)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);							//--- 카메라 위쪽 방향->벡터임(방향만) (음수하면 화면 상하거꾸로보임)

	glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::vec3 lightPos(2, 1, 0);
	glm::mat4 RotL = glm::rotate(glm::mat4(1.f), glm::radians(L_RotYAngle), glm::vec3(0, 1, 0));
	lightPos = RotL * glm::vec4(lightPos, 1.f);

	unsigned int lightPosLocation = glGetUniformLocation(shaderID, "lightPos");		//--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z + 0.1);
	unsigned int lightColorLocation = glGetUniformLocation(shaderID, "lightColor");	//--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	
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


	unsigned int viewPosLocation = glGetUniformLocation(shaderID, "viewPos");		//--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	GLuint	SelectColor = glGetUniformLocation(shaderID, "SelectColor");
	glUniform1i(SelectColor, 0);

	glBindVertexArray(floorVAO);
	glm::mat4 Scale{ glm::scale(glm::mat4(1.f), glm::vec3(2,2,2)) };
	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Scale)); //--- modelTransform 변수에 변환 값 적용하기
	glDrawArrays(GL_POLYGON, 0, 4);

	CRANE.Draw();
	LIGHT.Draw();

	glutSwapBuffers();							// 화면에 출력하기
}
