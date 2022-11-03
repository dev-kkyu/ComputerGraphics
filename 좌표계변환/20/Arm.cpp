#include "Arm.h"

Arm::Arm()
{
}

Arm::Arm(glm::vec3 Color, int i) : i(i), rotate(0.f), isrotDirec(false)
{
	this->Color = Color;
	update();
}

void Arm::update()
{
	if (isrotDirec) {
		rotate += 5.f;
		if (rotate > 30.f)
			isrotDirec = false;
	}
	else {
		rotate -= 5.f;
		if (rotate < -30.f)
			isrotDirec = true;
	}

	Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
	Scale = glm::scale(Unit, glm::vec3(0.03, 0.075, 0.03));

	Change = Scale * Trans;		// ���� �ø��� ���

	Trans = glm::translate(Unit, glm::vec3(0, -0.15, 0));
	Change = Trans * Change;	//�� �����ֱ����� ������ �����ְ�

	Rotate = glm::rotate(Unit, glm::radians(30.f), glm::vec3(0, 0, i * 1.f));
	Change = Rotate * Change;	//�����ش�.

	Rotate = glm::rotate(Unit, glm::radians(rotate), glm::vec3(i * 1.f, 0, 0));
	Change = Rotate * Change;	//�� ���� ��ǵ� ����

	Trans = glm::translate(Unit, glm::vec3(0, 0.15, 0));
	Change = Trans * Change;	//�ٽ� ����ġ �����ְ�

	Trans = glm::translate(Unit, glm::vec3(0, 0.25, 0));
	Change = Trans * Change;	// ���ϴ� ����

	Trans = glm::translate(Unit, glm::vec3(i * 0.15, 0, 0));
	Change = Trans * Change;	// ���� ���ϴ� ��ġ(���� ������)

	Rotate = glm::rotate(Unit, glm::radians(Direction), glm::vec3(0, 1, 0));
	Change = Rotate * Change;	//ĳ���� �����ֱ�

	Trans = glm::translate(Unit, Position);		// �� �̵���ġ�� ����
	Change = Trans * Change;
}
