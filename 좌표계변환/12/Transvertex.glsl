//#version 330 core
//
//layout(location = 0) in vec3 vPos; //--- ��ġ ����: attribute position 0
//layout(location = 1) in vec3 vColor; //--- �÷� ����: attribute position 1
//
//out vec3 outColor; //--- �÷��� fragment shader�� ���
//
//void main()
//{
//	gl_Position = vec4(vPos, 1.0);
//	outColor = vColor; //--- vertex data�κ��� ������ �÷� �Է��� ourColor�� ����
//}


#version 330 core

layout (location = 0) in vec3 vPos; //--- ���� ���α׷����� �޾ƿ� ���� ��ǥ��
layout (location = 1) in vec3 vColor;

out vec3 outColor;

uniform mat4 modelTransform; //--- �𵨸� ��ȯ ���: uniform ������ ����

void main()
{
	gl_Position = modelTransform * vec4(vPos, 1.0); //--- ��ǥ���� modelTransform ��ȯ�� �����Ѵ�.
	outColor = vColor;
}
