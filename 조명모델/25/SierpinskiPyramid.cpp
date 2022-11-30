#include "SierpinskiPyramid.h"

SierpinskiPyramid::SierpinskiPyramid(glm::vec3 Color) : Color{ Color }
{
}

void SierpinskiPyramid::DrawTri(glm::mat4 Change)
{
	glBindVertexArray(equilateralTriVAO);

	unsigned int objColorLocation = glGetUniformLocation(shaderID, "objectColor");	//--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, Color.r, Color.g, Color.b);

	GLuint model = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Change));

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void SierpinskiPyramid::DrawFace(int fractalNum, glm::mat4 Change)
{
	const glm::mat4 Unit(1.f);

	glm::mat4 sca = glm::scale(glm::mat4(1.f), glm::vec3(0.5, 0.5, 1));
	glm::mat4 tra[3] = {
		glm::translate(glm::mat4(1.f), glm::vec3(-0.5, 0, 0)),
		glm::translate(glm::mat4(1.f), glm::vec3(0.5, 0, 0)),
		glm::translate(glm::mat4(1.f), glm::vec3(0, sqrt(3) / 2.f, 0))
	};

	switch (fractalNum)
	{

	case 0:
		DrawTri(Change);
		break;
	case 1:
		for (int i = 0; i < 3; ++i) {
			glm::mat4 temp = Change * tra[i] * sca;
			DrawTri(temp);
		}
		break;
	case 2:
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				glm::mat4 temp = Change * tra[i] * sca * tra[j] * sca;
				DrawTri(temp);
			}
		}
		break;
	case 3:
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = 0; k < 3; ++k) {
					glm::mat4 temp = Change * tra[i] * sca * tra[j] * sca * tra[k] * sca;
					DrawTri(temp);
				}
			}
		}
		break;
	case 4:
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = 0; k < 3; ++k) {
					for (int l = 0; l < 3; ++l) {
						glm::mat4 temp = Change * tra[i] * sca * tra[j] * sca * tra[k] * sca * tra[l] * sca;
						DrawTri(temp);
					}
				}
			}
		}
		break;
	case 5:
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = 0; k < 3; ++k) {
					for (int l = 0; l < 3; ++l) {
						for (int m = 0; m < 3; ++m) {
							glm::mat4 temp = Change * tra[i] * sca * tra[j] * sca * tra[k] * sca * tra[l] * sca * tra[m] * sca;
							DrawTri(temp);
						}
					}
				}
			}
		}
		break;
	case 6:
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = 0; k < 3; ++k) {
					for (int l = 0; l < 3; ++l) {
						for (int m = 0; m < 3; ++m) {
							for (int n = 0; n < 3; ++n) {
								glm::mat4 temp = Change * tra[i] * sca * tra[j] * sca * tra[k] * sca * tra[l] * sca * tra[m] * sca * tra[n] * sca;
								DrawTri(temp);
							}
						}
					}
				}
			}
		}
		break;
	case 7:
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = 0; k < 3; ++k) {
					for (int l = 0; l < 3; ++l) {
						for (int m = 0; m < 3; ++m) {
							for (int n = 0; n < 3; ++n) {
								for (int o = 0; o < 3; ++o) {
									glm::mat4 temp = Change * tra[i] * sca * tra[j] * sca * tra[k] * sca * tra[l] * sca * tra[m] * sca * tra[n] * sca * tra[o] * sca;
									DrawTri(temp);
								}
							}
						}
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

void SierpinskiPyramid::Draw(int fractalNum)
{
	//네 면 그리기
	glm::mat4 Unit(1.f);
	glm::mat4 XRotate = glm::rotate(Unit, (float)atan2(1, sqrt(2)), glm::vec3(-1, 0, 0));
	glm::mat4 Trans = glm::translate(Unit, glm::vec3(0, 0, 1));

	for (int i = 0; i < 4; ++i) {
		glm::mat4 YRotate = glm::rotate(Unit, glm::radians(i * 90.f), glm::vec3(0, 1, 0));
		glm::mat4 Change = YRotate * Trans * XRotate;
		DrawFace(fractalNum, Change);
	}
}
