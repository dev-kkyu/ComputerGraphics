#include "Robot.h"

Robot::Robot() : head(Head(glm::vec3(191. / 255, 160. / 255, 237. / 255))), nose(Nose(glm::vec3(1.f, 108. / 255, 108. / 255))), body(Body(glm::vec3(243. / 255, 1, 72. / 255))),
	armL(Arm(glm::vec3(178. / 255, 204. / 255, 1.f), -1)), armR(Arm(glm::vec3(1.f, 193. / 255, 158. / 255), 1)),
	legL(Leg(glm::vec3(1.f, 193. / 255, 158. / 255), -1)), legR(Leg(glm::vec3(178. / 255, 204. / 255, 1.f), 1)),
	Position(glm::vec3(0.f, -1.f, 0.f)), Direction(0.f), 
	MASS(10), VELOCITY(25), Vel(VELOCITY)
{
}

void Robot::Jump()
{

	double F;

	if (isJump == 2) {
		isJump = 3;
		Vel = VELOCITY;
	}

	if (Vel > 0) {
		F = MASS * Vel * Vel;
	}
	else
		F = -MASS * Vel * Vel;

	Position.y += F / 100000.f;

	Vel -= 1;

	if (Position.y < -1.f) {
		Vel = VELOCITY;
		Position.y = -1.f;
		isJump = 0;
	}

}

void Robot::draw()
{
	nose.draw();
	head.draw();
	armL.draw();
	armR.draw();
	body.draw();
	legL.draw();
	legR.draw();
}

void Robot::update(unsigned char key)
{
	switch (key) {
	case 'w':
	case 'W':
		Position.z -= 0.01;
		if (Position.z < -1.f)
			Position.z = 1.f;
		Direction = 180.f;
		break;
	case 'a':
	case 'A':
		Position.x -= 0.01;
		if (Position.x < -1.f)
			Position.x = 1.f;
		Direction = 270.f;
		break;
	case 's':
	case 'S':
		Position.z += 0.01;
		if (Position.z > 1.f)
			Position.z = -1.f;
		Direction = 0.f;
		break;
	case 'd':
	case 'D':
		Position.x += 0.01;
		if (Position.x > 1.f)
			Position.x = -1.f;
		Direction = 90.f;
		break;
	case 'j':
	case 'J':
		Jump();
		break;
	}



	nose.keyIn(Position, Direction);
	head.keyIn(Position, Direction);
	armL.keyIn(Position, Direction);
	armR.keyIn(Position, Direction);
	body.keyIn(Position, Direction);
	legL.keyIn(Position, Direction);
	legR.keyIn(Position, Direction);
}
