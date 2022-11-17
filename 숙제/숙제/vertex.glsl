#version 330 core

layout (location = 0) in vec3 vPos; //--- 응용 프로그램에서 받아온 도형 좌표값
layout (location = 1) in vec3 vColor;

out vec3 outColor;

uniform mat4 modelTransform;			//모델링 변환
uniform mat4 viewTransform;				//뷰잉 변환(카메라)
uniform mat4 projectionTransform;		//투영 변환(직각, 원근)

void main()
{
	gl_Position =  projectionTransform * viewTransform * modelTransform * vec4(vPos, 1.0); //--- 좌표값에 modelTransform 변환을 적용한다.
	outColor = vColor;
}
