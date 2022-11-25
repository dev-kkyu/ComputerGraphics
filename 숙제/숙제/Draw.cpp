#include "Draw.h"

float C_movX, C_movZ;
float C_RotYAngle;

bool isMakeMountain;

World newWorld;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	if (isDepTest)
		glEnable(GL_DEPTH_TEST);		//은면제거
	else
		glDisable(GL_DEPTH_TEST);

	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);

	glEnable(GL_DITHER);        // 표면을 매끄럽게
	glEnable(GL_CULL_FACE);     // 컬링
	glEnable(GL_LINE_SMOOTH);   // 안티 앨리어싱
	glEnable(GL_POLYGON_SMOOTH);// 안티 앨리어싱
	glShadeModel(GL_SMOOTH);    // 부드러운 음영을 수행합니다.

	//glFrontFace(GL_CW);			//시계방향이 앞면으로 설정(박스 바깥쪽이 뒷면, 안쪽이 앞면)
	//glEnable(GL_CULL_FACE);		//컬링 설정해주기

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	unsigned int lightColorLocation = glGetUniformLocation(shaderID, "lightColor");	//--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 245 / 255., 214 / 255., 1.f);

	glViewport(0, 0, winWidth, winHeight);
	normalView();
	newWorld.Draw();



	glViewport(winWidth / 4 * 3 - 10, winHeight / 4 * 3 - 10, winWidth / 4, winHeight / 4);
	//기본 뷰 테두기 그리기
	resetView();
	drawLine();

	topView();
	newWorld.Draw();


	glutSwapBuffers();							// 화면에 출력하기

}

void resetView()
{
	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));

	unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
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
	// 원근투영

	//카메라
	newWorld.Camera(personView);
}

void topView()
{
	// 원근투영
	glm::mat4 projection = glm::ortho(-30.0f * ((float)winWidth / (float)winHeight), 30.0f * ((float)winWidth / (float)winHeight), -30.0f, 30.0f, -30.0f, 30.0f);
	GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	// 카메라
	glm::vec3 cameraPos = glm::vec3(C_movX, 10.f + C_movZ, 0.f); //--- 카메라 위치 (어디서 볼건지)
	glm::vec3 cameraDirection = glm::vec3(C_movX, 0.0f, 0.0f); //--- 카메라 바라보는 방향 (어디볼건지 하면될듯)
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, -1.0f); //--- 카메라 위쪽 방향->벡터임(방향만) (음수하면 화면 상하거꾸로보임)

	glm::mat4 cameraRotationY = glm::rotate(glm::mat4(1.f), glm::radians(MouseAngle.first), glm::vec3(0.f, 0.f, 1.f));

	glm::mat4 view;
	if (personView == 1 || personView == 3)
		view = cameraRotationY * glm::lookAt(cameraPos, cameraDirection, cameraUp);
	else
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
}
