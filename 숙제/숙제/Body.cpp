#include "Body.h"

Body::Body()
{
}

Body::Body(glm::vec3 Color)
{
	this->Color = Color;
	Update();
}

Body::~Body()
{
}

void Body::Update()
{
	glm::mat4 Scale;
	glm::mat4 Trans;
	//glm::mat4 Rotate;

	Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
	Scale = glm::scale(Unit, glm::vec3(0.15, 0.15, 0.15));	//������� 0.3, �ʺ� 0.15

	Change = Scale * Trans;		//���� �ø��� ���

	Trans = glm::translate(Unit, glm::vec3(0, 0.15, 0));
	Change = Trans * Change;	//�ٸ� ���� �÷���

	Rotate = glm::rotate(Unit, glm::radians(Direction), glm::vec3(0, 1, 0));
	Change = Rotate * Change;	//ĳ���� �����ֱ�



	Scale = glm::scale(Unit, glm::vec3(robotScale));					//�κ� ũ�� ��ȯ
	Change = Scale * Change;



	Trans = glm::translate(Unit, Position);
	Change = Trans * Change;	// �� ���������� �̵�
}
