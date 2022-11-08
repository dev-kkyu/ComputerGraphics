#include "Head.h"

Head::Head()
{
}

Head::Head(glm::vec3 Color)
{
	this->Color = Color;
	update();
}

Head::~Head()
{
}

void Head::update()
{
	Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
	Scale = glm::scale(Unit, glm::vec3(0.075, 0.05, 0.075));	//�Ӹ������� 0.1

	Change = Scale * Trans;		//���� �ø��� ���

	Trans = glm::translate(Unit, glm::vec3(0, 0.45, 0));
	Change = Trans * Change;	//�ٸ� ���� �÷���

	Rotate = glm::rotate(Unit, glm::radians(Direction), glm::vec3(0, 1, 0));
	Change = Rotate * Change;	//ĳ���� �����ֱ�

	Trans = glm::translate(Unit, Position);
	Change = Trans * Change;	// �� ���������� �̵�
}
