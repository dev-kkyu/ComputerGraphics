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

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	if (isDepTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	//glFrontFace(GL_CW);			//시계방향이 앞면으로 설정(박스 바깥쪽이 뒷면, 안쪽이 앞면)
	glEnable(GL_CULL_FACE);		//컬링 설정해주기

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// 원근투영
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)winWidth / (float)winHeight, 0.1f, 50.f);
	GLuint projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");	//--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	// 카메라
	glm::vec3 cameraPos = glm::vec3(0, 3, 15);		//--- 카메라 위치 (어디서 볼건지)
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);					//--- 카메라 바라보는 방향 (어디볼건지 하면될듯)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);							//--- 카메라 위쪽 방향->벡터임(방향만) (음수하면 화면 상하거꾸로보임)

	glm::mat4 Rotate = glm::rotate(glm::mat4(1.f), glm::radians(cameraRotAngle), glm::vec3(0, 1, 0));
	cameraPos = Rotate * glm::vec4(cameraPos, 1.f);

	glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	GLuint viewPosLocation = glGetUniformLocation(shaderID, "viewPos");		//--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);


	glBindVertexArray(floorVAO);
	glBindTexture(GL_TEXTURE_2D, floorTexture);

	GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");
	glUniform1i(selectColorLocation, 1);			// 텍스처모드

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.f), glm::vec3(8, 0, 8))));

	glDrawArrays(GL_QUADS, 0, 4);

	// 불투명한거 먼저
	LIGHT.Draw();
	Pry.Draw(FractalNum);

	if (isDrowSnow)
		for (Snow& snow : SNOW)
			snow.Draw();

	// 그다음 투명한거
	for (Cube& cube : CUBE)
		cube.Draw();

	glutSwapBuffers();							// 화면에 출력하기
}
