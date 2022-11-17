#include "Robot.h"

float robotScale = 2.5f;

Robot::Robot() :
	head(Head(glm::vec3(191. / 255, 160. / 255, 237. / 255))),
	nose(Nose(glm::vec3(1.f, 108. / 255, 108. / 255))),
	body(Body(glm::vec3(243. / 255, 1, 72. / 255))),
	armL(Arm(glm::vec3(178. / 255, 204. / 255, 1.f), -1)),
	armR(Arm(glm::vec3(1.f, 193. / 255, 158. / 255), 1)),
	legL(Leg(glm::vec3(1.f, 193. / 255, 158. / 255), -1)),
	legR(Leg(glm::vec3(178. / 255, 204. / 255, 1.f), 1)),

	Position(glm::vec3(0.f, 0.f, 0.f)),
	Direction(0.f)
{
}

Robot::Robot(glm::vec3 Position) : Position(Position)
{
}

void Robot::Draw()
{
	if (isRobotDraw) {
		nose.draw();
		head.draw();
		armL.draw();
		armR.draw();
		body.draw();
		legL.draw();
		legR.draw();
	}
}

void Robot::Update(float ObjPosX, float ObjPosY, float ObjPosZ)
{
	if (isRobotDraw) {
		Direction = -MouseAngle.first + 180.f;
		Position.x = ObjPosX;
		Position.y = ObjPosY;
		Position.z = ObjPosZ;

		nose.keyIn(Position, Direction);
		head.keyIn(Position, Direction);
		armL.keyIn(Position, Direction);
		armR.keyIn(Position, Direction);
		body.keyIn(Position, Direction);
		legL.keyIn(Position, Direction);
		legR.keyIn(Position, Direction);
	}
}

void Robot::setPos(glm::vec3 Position, float Direction)
{
	this->Position = Position;
	this->Direction = Direction;

	nose.keyIn(Position, Direction);
	head.keyIn(Position, Direction);
	armL.keyIn(Position, Direction);
	armR.keyIn(Position, Direction);
	body.keyIn(Position, Direction);
	legL.keyIn(Position, Direction);
	legR.keyIn(Position, Direction);
}

float Robot::getLeft()
{
	return Position.x - (0.15f * robotScale);
}

float Robot::getRight()
{
	return Position.x + (0.15f * robotScale);
}

float Robot::getBehind()
{
	return Position.z - (0.15f * robotScale);
}

float Robot::getFront()
{
	return Position.z + (0.15f * robotScale);
}

float Robot::getBottom()
{
	return Position.y;
}

float Robot::getTop()
{
	return 0.0f;				//계산하기 귀찮으니 대충 0으로 잡겠음(의미없는값)
}