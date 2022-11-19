#include "Nose.h"

Nose::Nose()
{
}

Nose::Nose(glm::vec3 Color)
{
	this->Color = Color;
	Update();
}

Nose::~Nose()
{
}

void Nose::Update()
{
	glm::mat4 Scale;
	glm::mat4 Trans;
	//glm::mat4 Rotate;

	Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
	Scale = glm::scale(Unit, glm::vec3(0.01, 0.01, 0.01));	//������ �ñ⶧���� �����غ����� y �÷���

	Change = Scale * Trans;		//���� �ø��� ���

	Trans = glm::translate(Unit, glm::vec3(0, 0.5, 0));
	Trans = Trans * glm::translate(Unit, glm::vec3(0, 0, 0.075));
	Change = Trans * Change;	//���ϴ� ��ġ�� �̵�

	Rotate = glm::rotate(Unit, glm::radians(Direction), glm::vec3(0, 1, 0));
	Change = Rotate * Change;	//ĳ���� �����ֱ�



	Scale = glm::scale(Unit, glm::vec3(robotScale));					//�κ� ũ�� ��ȯ
	Change = Scale * Change;



	Trans = glm::translate(Unit, Position);
	Change = Trans * Change;	// �� ���������� �̵�
}
