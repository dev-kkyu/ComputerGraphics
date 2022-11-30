#include "Light.h"

Light::Light() : Unit(glm::mat4(1.f)), Change(glm::mat4(1.f)),
	BoxColor(glm::vec3(0.8, 0.8, 0.8)), LightColor(glm::vec3(1.f)), distance(4.f)
{
}

void Light::EditValue(unsigned char key)
{
	switch (key) {
	case 'i':
		distance += 0.1f;
		break;
	case 'o':
		distance -= 0.1f;
		break;
	case '+':
		LightColor *= 1.05f;
		break;
	case '-':
		LightColor *= 0.95f;
		break;
	}
}

void Light::Update()
{
	glm::mat4 Scale = glm::scale(Unit, glm::vec3(0.3f, 0.3f, 0.3f));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(distance, 0, 0));
	glm::mat4 Rotate = glm::rotate(Unit, glm::radians(L_RotYAngle), glm::vec3(0, 1, 0));
	
	Change = Rotate * Trans * Scale;
}

void Light::Draw()
{
	glm::vec4 lightPos(distance - 0.5f, 0, 0, 1);
	glm::mat4 RotL = glm::rotate(glm::mat4(1.f), glm::radians(L_RotYAngle), glm::vec3(0, 1, 0));
	lightPos = RotL * lightPos;

	unsigned int lightPosLocation = glGetUniformLocation(shaderID, "lightPos");		//--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	unsigned int lightColorLocation = glGetUniformLocation(shaderID, "lightColor");	//--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, LightColor.r, LightColor.g, LightColor.b);

	glBindVertexArray(cubeVAO);

	unsigned int objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, BoxColor.r, BoxColor.g, BoxColor.b);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	glDrawArrays(GL_TRIANGLES, 0, cubevertex.size() * 3);
}
