#include "Head.h"

Head::Head()
{
}

Head::Head(glm::vec3 Color)
{
	this->Color = Color;
	Update();
}

Head::~Head()
{
}

void Head::Update()
{
	glm::mat4 Scale;
	glm::mat4 Trans;
	glm::mat4 Rotate;

	Trans = glm::translate(Unit, glm::vec3(0, 1.f, 0));
	Scale = glm::scale(Unit, glm::vec3(0.075, 0.05, 0.075));	//머리사이즈 0.1

	Change = Scale * Trans;		//위로 올리고 축소

	Trans = glm::translate(Unit, glm::vec3(0, 0.45, 0));
	Change = Trans * Change;	//다리 위로 올려줌

	Rotate = glm::rotate(Unit, glm::radians(Direction), glm::vec3(0, 1, 0));
	Change = Rotate * Change;	//캐릭터 돌려주기



	Scale = glm::scale(Unit, glm::vec3(robotScale));					//로봇 크기 변환
	Change = Scale * Change;



	Trans = glm::translate(Unit, Position);
	Change = Trans * Change;	// 내 포지션으로 이동
}
