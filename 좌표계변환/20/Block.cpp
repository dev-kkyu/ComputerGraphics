#include "Block.h"

Block::Block() : TopMiddlePoint(glm::vec3(0.f, -0.8f, 0.f))
{
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> GenXY(-0.8f, 0.8f);

	TopMiddlePoint.x = GenXY(dre);
	TopMiddlePoint.z = GenXY(dre);
}

void Block::draw()
{
	const glm::mat4 Unit(1.f);

	glm::mat4 Trans = glm::translate(Unit, glm::vec3(0, -1.f, 0));
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.2, 0.1, 0.2));

	glm::mat4 Change = Scale * Trans;		//아래로 내리고 축소

	Trans = glm::translate(Unit, TopMiddlePoint);
	Change = Trans * Change;	// 내 포지션으로 이동



	GLuint Color = glGetUniformLocation(shaderID, "Color");

	glUniform3f(Color, 0.7f, 0.7f, 0.7f);			//색깔 회색으로 해주자

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기

	glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 0)));
}

float Block::getLeft()
{
	return TopMiddlePoint.x - 0.2f;
}

float Block::getRight()
{
	return TopMiddlePoint.x + 0.2f;
}

float Block::getBehind()
{
	return TopMiddlePoint.z - 0.2f;
}

float Block::getFront()
{
	return TopMiddlePoint.z + 0.2f;
}

float Block::getBottom()
{
	return TopMiddlePoint.y - 0.2f;
}

float Block::getTop()
{
	return TopMiddlePoint.y;
}
