#include "Robot.h"

Robot::Robot() : head(Head(glm::vec3(191. / 255, 160. / 255, 237. / 255))), nose(Nose(glm::vec3(1.f, 108. / 255, 108. / 255))), body(Body(glm::vec3(243. / 255, 1, 72. / 255))),
	armL(Arm(glm::vec3(178. / 255, 204. / 255, 1.f), -1)), armR(Arm(glm::vec3(1.f, 193. / 255, 158. / 255), 1)),
	legL(Leg(glm::vec3(1.f, 193. / 255, 158. / 255), -1)), legR(Leg(glm::vec3(178. / 255, 204. / 255, 1.f), 1)),
	Position(glm::vec3(0.f, -1.f, 0.f)), Direction(0.f), 
	MASS(10), VELOCITY(25), Vel(VELOCITY)	//무게, 초기속도, 현재속도
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

	if (isCollide(*this, block1) || isCollide(*this, block2)) {
		Vel = VELOCITY;
		Position.y = -0.8f;
		isJump = 0;
		return;
	}

	if (Position.y <= -1.f) {
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
	bool isUP = false;

	if (isCollide2D(*this, block1) || isCollide2D(*this, block2))	//현재 블록 위에 있으면
		isUP = true;

	switch (key) {
	case 'w':
	case 'W':
		Position.z -= 0.01;
		if (Position.y < -0.8f && (isCollide2D(*this, block1) || isCollide2D(*this, block2))) {		//부딪치면 원복
			Position.z += 0.01;
		}
		if (Position.z < -1.f)
			Position.z = 1.f;
		Direction = 180.f;
		break;
	case 'a':
	case 'A':
		Position.x -= 0.01;
		if (Position.y < -0.8f && (isCollide2D(*this, block1) || isCollide2D(*this, block2))) {		//부딪치면 원복
			Position.x += 0.01;
		}
		if (Position.x < -1.f)
			Position.x = 1.f;
		Direction = 270.f;
		break;
	case 's':
	case 'S':
		Position.z += 0.01;
		if (Position.y < -0.8f && (isCollide2D(*this, block1) || isCollide2D(*this, block2))) {		//부딪치면 원복
			Position.z -= 0.01;
		}
		if (Position.z > 1.f)
			Position.z = -1.f;
		Direction = 0.f;
		break;
	case 'd':
	case 'D':
		Position.x += 0.01;
		if (Position.y < -0.8f && (isCollide2D(*this, block1) || isCollide2D(*this, block2))) {		//부딪치면 원복
			Position.x -= 0.01;
		}
		if (Position.x > 1.f)
			Position.x = -1.f;
		Direction = 90.f;
		break;
	case 'j':
	case 'J':
		Jump();
		break;
	case 'i':
	case 'I':
		Position = glm::vec3(0.f, -1.f, 0.f);
		Direction = 0.f;
		isJump = 0;
		Vel = 0;
		C_movX = 0.f;
		C_movZ = 0.f;
		C_RotYAngle = 0.f;
		isCRevolution = 0;
		isOpenF = false;
		break;
	}

	if (isUP && !(isCollide2D(*this, block1) || isCollide2D(*this, block2))) {	// 이동을 한 뒤 올라와있던게 이제 아니면 내린다.
		Vel = 0;
		isJump = 1;
	}

	nose.keyIn(Position, Direction);
	head.keyIn(Position, Direction);
	armL.keyIn(Position, Direction);
	armR.keyIn(Position, Direction);
	body.keyIn(Position, Direction);
	legL.keyIn(Position, Direction);
	legR.keyIn(Position, Direction);
}

Robot::~Robot()
{
}

float Robot::getLeft()
{
	return Position.x - 0.15f;
}

float Robot::getRight()
{
	return Position.x + 0.15f;
}

float Robot::getBehind()
{
	return Position.z - 0.15f;
}

float Robot::getFront()
{
	return Position.z + 0.15f;
}

float Robot::getBottom()
{
	return Position.y;
}

float Robot::getTop()
{
	return 0.0f;				//계산하기 귀찮으니 대충 0으로 잡겠음(의미없는값)
}

bool isCollide(Robot r1, Block r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	if (r1.getTop() < r2.getBottom() || r1.getBottom() > r2.getTop()) return false;
	return true;
}

bool isCollide2D(Robot r1, Block r2)
{
	if (r1.getRight() < r2.getLeft() || r1.getLeft() > r2.getRight()) return false;
	if (r1.getFront() < r2.getBehind() || r1.getBehind() > r2.getFront()) return false;
	return true;
}
