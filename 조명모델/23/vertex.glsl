#version 330 core

layout (location = 0) in vec3 vPos;		//--- 응용 프로그램에서 받아온 도형 좌표값
layout (location = 1) in vec3 vNormal;	//--- 법선벡터

out vec3 FragPos;						//--- 객체의 위치값을 프래그먼트 세이더로 보낸다.
out vec3 Normal;						//--- 노멀값을 프래그먼트 세이더로 보낸다.

uniform mat4 modelTransform;			//--- 모델링 변환
uniform mat4 viewTransform;				//--- 뷰잉 변환(카메라)
uniform mat4 projectionTransform;		//--- 투영 변환(직각, 원근)

void main()
{
	gl_Position =  projectionTransform * viewTransform * modelTransform * vec4(vPos, 1.0);	//--- 좌표값에 투영 * 카메라 * 모델 변환을 적용한다.

	FragPos = vec3(modelTransform * vec4(vPos, 1.0));	//--- 객체에 대한 조명 계산을 프래그먼트 셰이더에서 한다. 
														//--- 따라서 월드공간에 있는 버텍스 값을 프래그먼트 셰이더로 보낸다.
	Normal = mat3(transpose(inverse(modelTransform))) * vNormal;			//--- 노멀값을 프래그먼트 세이더로 보낸다.
}