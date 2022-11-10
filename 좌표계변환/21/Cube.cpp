#include "Cube.h"

Cube::Cube() : Object()
{
}

Cube::Cube(float scale, float zPos)
	: Object(scale)
{
	Color = glm::vec3(0.7f, 0.5f, 0.7f);
	Position = glm::vec3(0.f, -1.f + scale, zPos);
}

Cube::~Cube()
{
}

void Cube::FixUpdate()
{
	static int wayx = -1;
	static int wayy = -1;
	static const int mass = 2;
	static float velocityx = 1.f;
	static float velocityy = 1.f;

	if (!isFloorOpen) {
		//--------------------------------X기준 바닥일때----------------------------
		if ((CubeAngle % 360 < -180) || (CubeAngle % 360 > 0 && CubeAngle % 360 < 180)) {
			if (wayx != -1) {
				wayx = -1;
				velocityx = 1.f;
			}
			if (Position.x > -1.f + scale) {
				Position.x -= (mass * velocityx * velocityx) * 0.00001f;
				velocityx += 0.5f;
			}
			else {
				Position.x = -1.f + scale;
				velocityx = 1.f;
			}

		}
		if (CubeAngle % 360 > 180 || (CubeAngle % 360 < 0 && CubeAngle % 360 > -180)) {
			if (wayx != 1) {
				wayx = 1;
				velocityx = 1.f;
			}
			if (Position.x < 1.f - scale) {
				Position.x += (mass * velocityx * velocityx) * 0.00001f;
				velocityx += 0.5f;
			}
			else {
				Position.x = 1.f - scale;
				velocityx = 1.f;
			}
		}



		////--------------------------------Y기준 바닥일때----------------------------
		if ((CubeAngle % 360 > -90 && CubeAngle % 360 < 90) || (CubeAngle % 360 < -270 || CubeAngle % 360 > 270)) {
			if (wayy != -1) {
				wayy = -1;
				velocityy = 1.f;
			}
			if (Position.y > -1.f + scale) {
				Position.y -= (mass * velocityy * velocityy) * 0.00001f;
				velocityy += 0.5f;
			}
			else {
				Position.y = -1.f + scale;
				velocityy = 1.f;
			}

		}
		if ((CubeAngle % 360 < -90 && CubeAngle % 360 > -270) || (CubeAngle % 360 > 90 && CubeAngle % 360 < 270)) {
			if (wayy != 1) {
				wayy = 1;
				velocityy = 1.f;
			}
			if (Position.y < 1.f - scale) {
				Position.y += (mass * velocityy * velocityy) * 0.00001f;
				velocityy += 0.5f;
			}
			else {
				Position.y = 1.f - scale;
				velocityy = 1.f;
			}
		}
	}
}

void Cube::Update()
{
	FixUpdate();

	glm::mat4 Scale = glm::scale(Unit, glm::vec3(scale));
	glm::mat4 Trans = glm::translate(Unit, Position);
	glm::mat4 Rotate = glm::rotate(Unit, glm::radians((float)CubeAngle), glm::vec3(0.f, 0.f, 1.f));

	Change = Rotate * Trans * Scale;
}

void Cube::Draw()
{
	GLuint Color = glGetUniformLocation(shaderID, "Color");
	glUniform3f(Color, this->Color.r, this->Color.g, this->Color.b);

	GLuint modelLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Change)); //--- modelTransform 변수에 변환

	glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, (void*)(sizeof(float) * (0 * 0)));
}
