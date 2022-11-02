#include "circle.h"

float XAngle = 10.f;

float movX, movY, movZ;
float spinYangle;

const glm::mat4 Unit(1.f);
glm::mat4 Trans;
glm::mat4 Rotate;
glm::mat4 Xrotate;
glm::mat4 Yrotate;
glm::mat4 mov;

void Circle::outCircle(float spinAngle, float speed)
{
	glm::mat4 Scale;
	glm::mat4 Change;

	glm::mat4 Rot;
	glm::mat4 Spin;
	glm::mat4 miniRotate;
	glm::mat4 revminiRotate;
	glm::mat4 miniSpin;

	Rot = glm::rotate(Unit, glm::radians(-inAngle), glm::vec3(0.0, 1.0, 0.0));	//--- 회전 행렬 -> Y
	Rot = glm::rotate(Rot, glm::radians(-Angle), glm::vec3(0.0, 0.0, 1.0));	//--- 회전 행렬 -> Z

	Spin = glm::rotate(Unit, glm::radians(-outAngle * speed), glm::vec3(0.0, 1.0, 0.0));
	Spin = glm::translate(Spin, glm::vec3(0.25, 0.0, 0.0));

	miniRotate = glm::rotate(Unit, glm::radians(spinAngle), glm::vec3(0.0, 0.0, 1.0));	//--- 회전 행렬 -> Z

	revminiRotate = glm::rotate(Unit, glm::radians(outAngle * speed), glm::vec3(0.0, 1.0, 0.0));	//--- 회전 행렬 -> Z
	revminiRotate = glm::rotate(revminiRotate, glm::radians(-spinAngle), glm::vec3(0.0, 0.0, 1.0));	//--- 회전 행렬 -> Z

	miniSpin = glm::rotate(Unit, glm::radians(outAngle * 3.f * speed), glm::vec3(0.0, 1.0, 0.0));
	miniSpin = glm::translate(miniSpin, glm::vec3(0.125, 0.0, 0.0));


	//원 따라 움직이는 구 주위 평면 그리기-----------------------------------------------------------------------------------------------------------------
	glBindVertexArray(cVAO);

	Scale = glm::scale(Unit, glm::vec3(0.5, 0.5, 0.5));					//--- 신축 행렬
	Change = Xrotate * mov * Yrotate * Rotate * Trans * Rot * miniRotate * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기
	glDrawArrays(GL_LINE_LOOP, 0, 360);

	//원 따라 움직이는 구 주위 평면을 공전하는 원-----------------------------------------------------------------------------------------------------------------

	glBindVertexArray(VAO);

	Scale = glm::scale(Unit, glm::vec3(0.01, 0.01, 0.01));					//--- 신축 행렬
	Change = Xrotate * mov * Yrotate * Rotate * Trans * Rot * miniRotate * Spin * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기

	glBufferData(GL_ARRAY_BUFFER, rndColor.size() * sizeof(glm::vec3), &rndColor[0], GL_STATIC_DRAW);
	//SetColor(2);
	//for (int i = 0; i < face.size(); ++i)
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));
	glDrawElements(GL_TRIANGLES, 3 * face.size(), GL_UNSIGNED_INT, (void*)0);

	//원 따라 움직이는 구 주위 평면 따라 움직이는 구 주위 평면 그리기-----------------------------------------------------------------------------------------------------------------
	glBindVertexArray(cVAO);

	Scale = glm::scale(Unit, glm::vec3(0.25, 0.25, 0.25));					//--- 신축 행렬
	Change = Xrotate * mov * Yrotate * Rotate * Trans * Rot * miniRotate * Spin * revminiRotate * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기
	glDrawArrays(GL_LINE_LOOP, 0, 360);

	//원 따라 움직이는 구 주위 평면 따라 움직이는 구 주위 평면을 따라 움직이는 구-----------------------------------------------------------------------------------------------------------------

	glBindVertexArray(VAO);

	Scale = glm::scale(Unit, glm::vec3(0.005, 0.005, 0.005));					//--- 신축 행렬
	Change = Xrotate * mov * Yrotate * Rotate * Trans * Rot * miniRotate * Spin * revminiRotate * miniSpin * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기

	//glBufferData(GL_ARRAY_BUFFER, rndColor.size() * sizeof(glm::vec3), &rndColor[0], GL_STATIC_DRAW);
	SetColor(1);
	//for (int i = 0; i < face.size(); ++i)
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));
	glDrawElements(GL_TRIANGLES, 3 * face.size(), GL_UNSIGNED_INT, (void*)0);
}

Circle::Circle() = default;

Circle::Circle(float Angle) : Angle(Angle), inAngle{}, outAngle{}, modelLocation{} {}

void Circle::draw()
{
	modelLocation = glGetUniformLocation(shaderID, "modelTransform");

	glm::mat4 Scale;
	glm::mat4 Change;

	mov = glm::translate(Unit, glm::vec3(movX, movY, movZ));
	Yrotate = glm::rotate(Unit, glm::radians(spinYangle), glm::vec3(0.0, 1.0, 0.0));

	Trans = glm::translate(Unit, glm::vec3(0.75, 0.0, 0.0));

	Rotate = glm::rotate(Unit, glm::radians(Angle), glm::vec3(0.0, 0.0, 1.0));	//--- 회전 행렬 -> Z
	Rotate = glm::rotate(Rotate, glm::radians(inAngle), glm::vec3(0.0, 1.0, 0.0));	//--- 회전 행렬 -> Y

	Xrotate = glm::rotate(Unit, glm::radians(XAngle), glm::vec3(1.0, 0.0, 0.0));
	Xrotate = glm::rotate(Xrotate, glm::radians(3.f), glm::vec3(0.0, 1.0, 0.0));

	//원(평면) 그리기---------------------------------------------------------------------------------------------------------
	glBindVertexArray(cVAO);

	Scale = glm::scale(Unit, glm::vec3(1.5, 1.5, 1.5));					//--- 신축 행렬
	Change = Xrotate * mov * Yrotate * Rotate * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기

	glDrawArrays(GL_LINE_LOOP, 0, 360);

	//원 따라 움직이는 구 그리기-----------------------------------------------------------------------------------------------------------------
	glBindVertexArray(VAO);

	Scale = glm::scale(Unit, glm::vec3(0.02, 0.02, 0.02));					//--- 신축 행렬
	Change = Xrotate * mov * Yrotate * Rotate * Trans * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기

	SetColor(0);
	//for (int i = 0; i < face.size(); ++i)
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));
	glDrawElements(GL_TRIANGLES, 3 * face.size(), GL_UNSIGNED_INT, (void*)0);
	

	outCircle(0.f, 1.f);
	outCircle(-45.f, 1.3f);
	outCircle(45.f, 0.7f);
}

void Circle::addAngle(float in, float out)
{
	inAngle += in;
	outAngle += out;
}

void SetColor(int index)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, vcolor[index].size() * sizeof(glm::vec3), &vcolor[index][0], GL_STATIC_DRAW);
}
