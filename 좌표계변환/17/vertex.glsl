#version 330 core

layout (location = 0) in vec3 vPos; //--- ���� ���α׷����� �޾ƿ� ���� ��ǥ��
layout (location = 1) in vec3 vColor;

out vec3 outColor;

uniform mat4 modelTransform; //--- �𵨸� ��ȯ ���: uniform ������ ����
uniform mat4 projectionTransform;

void main()
{
	gl_Position = projectionTransform * modelTransform * vec4(vPos, 1.0); //--- ��ǥ���� modelTransform ��ȯ�� �����Ѵ�.
	outColor = vColor;
}
