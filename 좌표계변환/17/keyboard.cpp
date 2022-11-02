#include "keyboard.h"

int spinY;

GLvoid Keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'p': {
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f); //--- 투영 공간을 [-100.0, 100.0] 공간으로 설정

		unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
			break;
	case 'P': {
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0)); //--- 공간을 약간 뒤로 미뤄줌
		unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
	case 'm':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'M':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'w':
		movY += 0.05f;
		break;
	case 'a':
		movX -= 0.05f;
		break;
	case 's':
		movY -= 0.05f;
		break;
	case 'd':
		movX += 0.05f;
		break;
	case 'z':
		movZ += 0.05f;
		break;
	case 'x':
		movZ -= 0.05f;
		break;
	case 'y':
		spinY = 1;
		break;
	case 'Y':
		spinY = -1;
		break;
	case 'r':
		spinY = 0;
		break;







	case 'h':
		isDepTest = !isDepTest;
		break;
	}

	glutPostRedisplay();
}