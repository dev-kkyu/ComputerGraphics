#include "Object.h"

Object::Object()
	: Unit(1.f)
{
}

Object::~Object()
{
}

Object::Object(float scale)
	: Unit(1.f), scale(scale)
{
}

void Object::Drop()
{
	Position.y -= 0.05f;
}

