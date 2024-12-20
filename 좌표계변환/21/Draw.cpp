#include "Draw.h"

float C_movX, C_movZ;
float C_RotYAngle;

int CubeAngle;

vector<Sphere*> sphere;
Cube cube[3]{ Cube(0.08f, +0.18f), Cube(0.1f, 0.f), Cube(0.12f, -0.22f) };

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	const glm::mat4 Unit(1.0f);

	if (isDepTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glFrontFace(GL_CW);			//시계방향을 뒷면으로 설정(박스 바깥쪽이 뒷면, 안쪽이 앞면)
	glEnable(GL_CULL_FACE);		//컬링 설정해주기

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// 카메라
	glm::vec3 cameraPos = glm::vec3(C_movX, 0.f, 4.f + C_movZ); //--- 카메라 위치 (어디서 볼건지)
	glm::vec3 cameraDirection = glm::vec3(C_movX, 0.0f, 0.0f); //--- 카메라 바라보는 방향 (어디볼건지 하면될듯)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향->벡터임(방향만) (음수하면 화면 상하거꾸로보임)

	glm::mat4 cameraRevolution = glm::rotate(glm::mat4(1.f), glm::radians(C_RotYAngle), glm::vec3(0.f, 1.f, 0.f));

	glm::mat4 view = glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraRevolution;

	GLuint viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));




	//-----------------------------------------------------모델변환-------------------------------------------------------
	//-----------------------------------------------------상자그리기-----------------------------------------------------
	//glm::mat4 Trans;
	//glm::mat4 Rotate;
	//glm::mat4 Scale;
	//glm::mat4 Change;
	glm::mat4 RotCube = glm::rotate(Unit, glm::radians((float)CubeAngle), glm::vec3(0.f, 0.f, 1.f));

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(RotCube)); //--- modelTransform 변수에 변환 값 적용하기

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

	if (isFloorOpen) {
		static float DropVal = 0.f;
		glm::mat4 DropTrans = glm::translate(Unit, glm::vec3(0.f, DropVal, 0.f));
		RotCube = glm::rotate(Unit, glm::radians((float)CubeAngle), glm::vec3(0.f, 0.f, 1.f));

		glm::mat4 Change = RotCube * DropTrans;

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기
		DropVal -= 0.05f;
	}

	glUniform3f(Color, 1.f, 1.f, 1.f);
	glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*)(sizeof(float) * (4 * 3 * 2)));		//아래쪽

	//glUniform3f(Color, 0.8f, 0.7f, 1.f);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(float) * (5 * 3 * 2)));		//앞쪽

	glDisable(GL_CULL_FACE);		//컬링 설정 끄기




	//-----------------------------------------------------모델변환-------------------------------------------------------
	//-----------------------------------------------------상자그리기-----------------------------------------------------
	for (Sphere*& sph : sphere) {
		sph->Draw();
	}

	for (Cube& cub : cube) {
		cub.Draw();
	}

	glutSwapBuffers();							// 화면에 출력하기
}