#version 330 core

layout (location = 0) in vec3 vPos; //--- ���� ���α׷����� �޾ƿ� ���� ��ǥ��

out vec3 outColor;

uniform mat4 modelTransform;			//�𵨸� ��ȯ
uniform mat4 viewTransform;				//���� ��ȯ(ī�޶�)
uniform mat4 projectionTransform;		//���� ��ȯ(����, ����)

void main()
{
	gl_Position =  projectionTransform * viewTransform * modelTransform * vec4(vPos, 1.0); //--- ��ǥ���� modelTransform ��ȯ�� �����Ѵ�.
}
