#include "Object.h"

Object::Object()
	: Unit(glm::mat4(1.f)), Change(glm::mat4(1.f)), Rotate(glm::mat4(1.f))
{
}

void Object::Update()
{
	FixUpdate();

	Change = Rotate * Change;
}

void Object::Draw()
{
}
