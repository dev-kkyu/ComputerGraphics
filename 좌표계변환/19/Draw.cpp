#include "Draw.h"

float transZ = 0.f, rotY = 0.f, rotX = 0.f;

float CAngle_Rotation = 0.f;
float CAngle_RevolutionW = 0.f;
float CAngle_RevolutionLok = 0.f;

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	if (isDepTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//���ʾƷ� �׸���
	glViewport(10, 10, winWidth / 2, winHeight / 2);

	//�⺻ �� �׵α� �׸���
	resetView();
	glBindVertexArray(rectVAO);
	glLineWidth(4.f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glLineWidth(1.f);

	//���� �Ʒ� �⺻ �� �׸���
	normalView();
	drawView();

	
	//������ �� �׸���
	glViewport(winWidth / 2 + 50, winHeight / 2 + 50, winWidth/2 - 100, winHeight/2 - 100);
	//�⺻ �� �׵α� �׸���
	resetView();
	glBindVertexArray(rectVAO);
	glLineWidth(4.f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glLineWidth(1.f);

	//������ �� �⺻ �� �׸���
	topView();
	drawView();


	//������ �Ʒ� �׸���
	glViewport(winWidth / 2 + 50, 50, winWidth / 2 - 100, winHeight / 2 - 100);
	//�⺻ �� �׵α� �׸���
	resetView();
	glBindVertexArray(rectVAO);
	glLineWidth(4.f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glLineWidth(1.f);

	//������ �Ʒ� �⺻ �� �׸���
	frontView();
	drawView();


	//for (int i = 0; i < face[diag1].size(); ++i)
	//	glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(sizeof(float) * (i * 3)));

	glutSwapBuffers();							// ȭ�鿡 ����ϱ�
}

void drawView()
{
	const glm::mat4 Unit(1.0f);
	glm::mat4 Trans;
	glm::mat4 Rotate;
	glm::mat4 Scale;
	glm::mat4 Change;

	glm::mat4 midTrans;
	glm::mat4 TransArm;

	glm::mat4 ZTrans;
	glm::mat4 YRotate;
	glm::mat4 XRotate;

	glm::mat4 tempTrans;
	glm::mat4 tempTransres;




	Rotate = glm::rotate(Unit, glm::radians(0.0f), glm::vec3(1.0, 0.0, 0.0));

	ZTrans = glm::translate(Unit, glm::vec3(0.0, 0.0, transZ));
	YRotate = glm::rotate(Unit, glm::radians(rotY), glm::vec3(0.f, 1.f, 0.f));

	//�� ȸ����ų�� �������ٰ�
	tempTrans = glm::translate(Unit, glm::vec3(0.0, 0.1, 0.0));
	tempTransres = glm::translate(Unit, glm::vec3(0.0, -0.1, 0.0));

	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------��ǥ�� �׸���------------------------------------------------------------
	glBindVertexArray(crossVAO);					//���ε�

	//Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 0.0));						//--- �̵� ���
	//Rz = glm::rotate(Rz, glm::radians(30.f), glm::vec3(1.0, 0.0, 0.0));	//--- ȸ�� ��� -> X
	//scale = glm::scale(scale, glm::vec3(1.0, 1.0, 1.0));					//--- ���� ���

	Change = Rotate;

	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glDrawArrays(GL_LINES, 0, 6);


	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------�ٴ� �׸���------------------------------------------------------------
	glBindVertexArray(floorVAO);					//���ε�

	Change = glm::translate(Unit, glm::vec3(0.0, -0.0001, 0.0));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glDrawArrays(GL_POLYGON, 0, 4);

	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------�Ʒ� ��ü �׸���------------------------------------------------------------
	glBindVertexArray(VAO);

	Trans = glm::translate(Unit, glm::vec3(0.0, 1.0, 0.0));
	Scale = glm::scale(Unit, glm::vec3(0.25, 0.1, 0.25));

	Change = Rotate * ZTrans * Scale * Trans;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 3)));


	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------�߰� ��ü �׸���------------------------------------------------------------
	glBindVertexArray(VAO);

	Trans = glm::translate(Unit, glm::vec3(0.0, 5.0, 0.0));			//���� ���� ������ 2�� �����̸� �� ����, ������ ���� ������ ������ �ѱ���ġ��ŭ ����?
	Scale = glm::scale(Unit, glm::vec3(0.125, 0.05, 0.125));
	//Scale = glm::scale(Unit, glm::vec3(0.25, 0.1, 0.25));

	Change = Rotate * ZTrans * YRotate * Scale * Trans;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 3)));


	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------��1 �׸���------------------------------------------------------------
	glBindVertexArray(VAO);

	TransArm = glm::translate(Unit, glm::vec3(0.05, 0.0, 0.0));
	Trans = glm::translate(Unit, glm::vec3(0.0, 0.4, 0.0));			// ���� �ø��°�
	Scale = glm::scale(Unit, glm::vec3(0.04, 0.1, 0.04));
	XRotate = glm::rotate(Unit, glm::radians(rotX), glm::vec3(1.f, 0.f, 0.f));

	Change = Rotate * ZTrans * YRotate * TransArm * Trans * tempTransres * XRotate * tempTrans * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 3)));


	//---------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------��2 �׸���------------------------------------------------------------
	glBindVertexArray(VAO);

	TransArm = glm::translate(Unit, glm::vec3(-0.05, 0.0, 0.0));
	XRotate = glm::rotate(Unit, glm::radians(rotX), glm::vec3(-1.f, 0.f, 0.f));

	Change = Rotate * ZTrans * YRotate * TransArm * Trans * tempTransres * XRotate * tempTrans * Scale;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 3)));
}

void resetView()
{
	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));

	unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));

	unsigned int modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
}

void normalView()
{
	const glm::mat4 Unit(1.0f);
	// ������ȯ
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- ������ �ణ �ڷ� �̷���
	unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);


	// ���׺�ȯ(ī�޶�)
	glm::mat4 cameraRotation = glm::rotate(Unit, glm::radians(CAngle_Rotation), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 cameraRevolutionW = glm::rotate(Unit, glm::radians(CAngle_RevolutionW), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 cameraRevolutionLok = glm::rotate(Unit, glm::radians(CAngle_RevolutionLok), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 cameraTrans = glm::translate(Unit, glm::vec3(movCX, 0.0f, 0.0f));
	glm::mat4 cameraTransrev = glm::translate(Unit, glm::vec3(-movCX, 0.0f, 0.0f));

	glm::vec3 cameraPos = glm::vec3(movCX, (3.0f + movCZ) * 0.4f, 3.0f + movCZ); //--- ī�޶� ��ġ (��� ������)
	glm::vec3 cameraDirection = glm::vec3(movCX, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ���� (��𺼰��� �ϸ�ɵ�)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����->������(���⸸) (�����ϸ� ȭ�� ���ϰŲٷκ���)
	
	glm::mat4 view;

	view = cameraRotation * glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraRevolutionW;
	if (isCRevolutionLok) {
		view = /*cameraRotation * */glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraTrans * cameraRevolutionLok * cameraTransrev;
	}

	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}

void topView()
{
	const glm::mat4 Unit(1.0f);
	// ������ȯ
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- ������ �ణ �ڷ� �̷���
	unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);


	// ���׺�ȯ(ī�޶�)
	glm::mat4 cameraRotation = glm::rotate(Unit, glm::radians(CAngle_Rotation), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 cameraRevolutionW = glm::rotate(Unit, glm::radians(CAngle_RevolutionW), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 cameraRevolutionLok = glm::rotate(Unit, glm::radians(CAngle_RevolutionLok), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 cameraTrans = glm::translate(Unit, glm::vec3(movCX, 0.0f, 0.0f));
	glm::mat4 cameraTransrev = glm::translate(Unit, glm::vec3(-movCX, 0.0f, 0.0f));

	glm::vec3 cameraPos = glm::vec3(movCX, movCZ + 3.f, 0.0); //--- ī�޶� ��ġ (��� ������)
	glm::vec3 cameraDirection = glm::vec3(movCX, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ���� (��𺼰��� �ϸ�ɵ�)
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, -1.0f); //--- ī�޶� ���� ����->������(���⸸) (�����ϸ� ȭ�� ���ϰŲٷκ���)

	glm::mat4 view;

	view = cameraRotation * glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraRevolutionW;
	if (isCRevolutionLok) {
		view = /*cameraRotation * */glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraTrans * cameraRevolutionLok * cameraTransrev;
	}

	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}

void frontView()
{
	const glm::mat4 Unit(1.0f);
	// ������ȯ
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- ������ �ణ �ڷ� �̷���
	unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);


	// ���׺�ȯ(ī�޶�)
	//glm::vec3 cameraPos = glm::vec3(movCX, 0.f, 3.f); //--- ī�޶� ��ġ (��� ������)
	//glm::vec3 cameraDirection = glm::vec3(movCX, 0.f, 0.f); //--- ī�޶� �ٶ󺸴� ���� (��𺼰��� �ϸ�ɵ�)
	//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����->������(���⸸) (�����ϸ� ȭ�� ���ϰŲٷκ���)
	//glm::mat4 view = glm::mat4(1.0f);
	//view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	//view = glm::rotate(view, glm::radians(CAngle_Rotation), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 cameraRotation = glm::rotate(Unit, glm::radians(CAngle_Rotation), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 cameraRevolutionW = glm::rotate(Unit, glm::radians(CAngle_RevolutionW), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 cameraRevolutionLok = glm::rotate(Unit, glm::radians(CAngle_RevolutionLok), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 cameraTrans = glm::translate(Unit, glm::vec3(movCX, 0.0f, 0.0f));
	glm::mat4 cameraTransrev = glm::translate(Unit, glm::vec3(-movCX, 0.0f, 0.0f));

	glm::vec3 cameraPos = glm::vec3(movCX, 0.f, 3.0f + movCZ); //--- ī�޶� ��ġ (��� ������)
	glm::vec3 cameraDirection = glm::vec3(movCX, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ���� (��𺼰��� �ϸ�ɵ�)
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����->������(���⸸) (�����ϸ� ȭ�� ���ϰŲٷκ���)

	glm::mat4 view;

	view = cameraRotation * glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraRevolutionW;
	if (isCRevolutionLok) {
		view = /*cameraRotation * */glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraTrans * cameraRevolutionLok * cameraTransrev;
	}

	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}
