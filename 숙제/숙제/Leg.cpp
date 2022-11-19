#include "Leg.h"

Leg::Leg() : i(i)
{
}

Leg::Leg(glm::vec3 Color, int i) : i(i), rotate(0.f), isrotDirec(false)
{
	this->Color = Color;
	Update();
}

Leg::~Leg()
{
}

void Leg::Update()
{
	if (isW || isA || isS || isD) {
		if (isrotDirec) {
			rotate += 2.5f;
			if (rotate > 30.f)
				isrotDirec = false;
		}
		else {
			rotate -= 2.5f;
			if (rotate < -30.f)
				isrotDirec = true;
		}
	}

	glm::mat4 Scale;
	glm::mat4 Trans;
	//glm::mat4 Rotate;

	Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
	Scale = glm::scale(Unit, glm::vec3(0.04, 0.075, 0.04));	//�ٸ����� 0.15
	
	Change = Scale * Trans;		// ���� �ø��� ���

	Trans = glm::translate(Unit, glm::vec3(0, -0.15, 0));
	Change = Trans * Change;	//�ٸ� ���� ���� ������ �����ְ�

	Rotate = glm::rotate(Unit, glm::radians(rotate), glm::vec3(i * -1.f, 0, 0));
	Change = Rotate * Change;	//�ٸ� ���� ��� ����

	Trans = glm::translate(Unit, glm::vec3(0, 0.15, 0));
	Change = Trans * Change;	//�ٽ� ����ġ �����ְ�

	Trans = glm::translate(Unit, glm::vec3(i * 0.07, 0, 0));	
	Change = Trans * Change;	// ���� ���ϴ� ��ġ(���� ������

	Rotate = glm::rotate(Unit, glm::radians(Direction), glm::vec3(0, 1, 0));
	Change = Rotate * Change;	//ĳ���� �����ֱ�



	Scale = glm::scale(Unit, glm::vec3(robotScale));					//�κ� ũ�� ��ȯ
	Change = Scale * Change;



	Trans = glm::translate(Unit, Position);		// �� �̵���ġ�� ����
	Change = Trans * Change;
}
