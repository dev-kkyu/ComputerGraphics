#include "RobotParts.h"

RobotParts::RobotParts() : Unit(glm::mat4(1.f)), Scale(glm::mat4(1.f)), Trans(glm::mat4(1.f)),
	Rotate(glm::mat4(1.f)), Change(glm::mat4(1.f)), Color(glm::vec3(1.f)), Position(glm::vec3(0.f, -1.f, 0.f)), Direction(0.f)
{
}

void RobotParts::draw()
{
	GLuint Color = glGetUniformLocation(shaderID, "Color");

	glUniform3f(Color, this->Color.r, this->Color.g, this->Color.b);

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환 값 적용하기

	glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 0)));
}

void RobotParts::keyIn(unsigned char key)
{
	switch (key) {
	case 'w':
	case 'W':
		Position.z -= 0.02;
		if (Position.z < -1.f)
			Position.z = 1.f;
		Direction = 180.f;
		break;
	case 'a':
	case 'A':
		Position.x -= 0.02;
		if (Position.x < -1.f)
			Position.x = 1.f;
		Direction = 270.f;
		break;
	case 's':
	case 'S':
		Position.z += 0.02;
		if (Position.z > 1.f)
			Position.z = -1.f;
		Direction = 0.f;
		break;
	case 'd':
	case 'D':
		Position.x += 0.02;
		if (Position.x > 1.f)
			Position.x = -1.f;
		Direction = 90.f;
		break;

	case 'j':
	case 'J':
		break;
	}
	update();
}

void RobotParts::update()
{
}
