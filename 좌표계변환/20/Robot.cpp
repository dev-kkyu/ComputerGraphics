#include "Robot.h"

Robot::Robot() : head(Head(glm::vec3(191. / 255, 160. / 255, 237. / 255))), nose(Nose(glm::vec3(1.f, 108. / 255, 108. / 255))), body(Body(glm::vec3(243. / 255, 1, 72. / 255))),
	armL(Arm(glm::vec3(178. / 255, 204. / 255, 1.f), -1)), armR(Arm(glm::vec3(1.f, 193. / 255, 158. / 255), 1)), legL(Leg(glm::vec3(1.f, 193. / 255, 158. / 255), -1)), legR(Leg(glm::vec3(178. / 255, 204. / 255, 1.f), 1))
{
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
	nose.keyIn(key);
	head.keyIn(key);
	armL.keyIn(key);
	armR.keyIn(key);
	body.keyIn(key);
	legL.keyIn(key);
	legR.keyIn(key);
}
