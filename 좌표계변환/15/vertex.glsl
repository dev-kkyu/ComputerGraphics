#version 330 core

layout(location = 0) in vec3 vPos; //--- 위치 변수: attribute position 0
layout(location = 1) in vec3 vColor; //--- 컬러 변수: attribute position 1

out vec3 outColor; //--- 컬러를 fragment shader로 출력

void main()
{
	gl_Position = vec4(vPos, 1.0);
	outColor = vColor; //--- vertex data로부터 가져온 컬러 입력을 ourColor에 설정
}