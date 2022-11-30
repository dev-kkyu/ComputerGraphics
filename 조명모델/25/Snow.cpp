#include "Snow.h"

Snow::Snow() : Color{ glm::vec3(1) }, Change(glm::mat4(1.f))
{
	random_device rd;
	default_random_engine dre{ rd() };
	uniform_real_distribution<float> spdUrd{ 0.5f,1.f };
	uniform_real_distribution<float> scaUrd{ 0.01, 0.05 };
	uniform_real_distribution<float> posUrd{ -2.f, 2.f };

	Speed = spdUrd(dre);
	Scale = scaUrd(dre);
	Position = glm::vec3(posUrd(dre), posUrd(dre), posUrd(dre));
}

void Snow::Update()
{
	Position.y -= Speed / 100.f;
	if (Position.y <= 0.f)
		Position.y = 3.f;

	Change = glm::translate(glm::mat4(1.f), Position) * glm::scale(glm::mat4(1.f), glm::vec3(Scale));
}

void Snow::Draw()
{
	glBindVertexArray(sphereVAO);

	unsigned int objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, Color.r, Color.g, Color.b);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	glDrawArrays(GL_TRIANGLES, 0, spherevertex.size() * 3);
}
