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
	Scale = glm::scale(Unit, glm::vec3(0.15, 0.15, 0.15));	//몸통길이 0.3

	Change = Scale * Trans;		//위로 올리고 축소

	Trans = glm::translate(Unit, glm::vec3(0, 0.15, 0));
	Change = Trans * Change;	//다리 위로 올려줌

	Rotate = glm::rotate(Unit, glm::radians(Direction), glm::vec3(0, 1, 0));
	Change = Rotate * Change;	//캐릭터 돌려주기

	Trans = glm::translate(Unit, Position);
	Change = Trans * Change;	// 내 포지션으로 이동
}
