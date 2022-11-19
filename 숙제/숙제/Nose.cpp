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
	Scale = glm::scale(Unit, glm::vec3(0.01, 0.01, 0.01));	//위에서 봤기때문에 납작해보여서 y 늘려줌

	Change = Scale * Trans;		//위로 올리고 축소

	Trans = glm::translate(Unit, glm::vec3(0, 0.5, 0));
	Trans = Trans * glm::translate(Unit, glm::vec3(0, 0, 0.075));
	Change = Trans * Change;	//원하는 위치로 이동

	Rotate = glm::rotate(Unit, glm::radians(Direction), glm::vec3(0, 1, 0));
	Change = Rotate * Change;	//캐릭터 돌려주기



	Scale = glm::scale(Unit, glm::vec3(robotScale));					//로봇 크기 변환
	Change = Scale * Change;



	Trans = glm::translate(Unit, Position);
	Change = Trans * Change;	// 내 포지션으로 이동
}
