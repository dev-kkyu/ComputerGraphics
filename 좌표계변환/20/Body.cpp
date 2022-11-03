#include "Body.h"

Body::Body()
{
}

Body::Body(glm::vec3 Color)
{
	this->Color = Color;
	update();
}

void Body::update()
{
	Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
	Scale = glm::scale(Unit, glm::vec3(0.15, 0.15, 0.15));	//������� 0.3

	Change = Scale * Trans;		//���� �ø��� ���

	Trans = glm::translate(Unit, glm::vec3(0, 0.15, 0));
	Change = Trans * Change;	//�ٸ� ���� �÷���

	Rotate = glm::rotate(Unit, glm::radians(Direction), glm::vec3(0, 1, 0));
	Change = Rotate * Change;	//ĳ���� �����ֱ�

	Trans = glm::translate(Unit, Position);
	Change = Trans * Change;	// �� ���������� �̵�
}
