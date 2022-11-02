//#version 330 core
//
//layout(location = 0) in vec3 vPos; //--- 위치 변수: attribute position 0
//layout(location = 1) in vec3 vColor; //--- 컬러 변수: attribute position 1
//
//out vec3 outColor; //--- 컬러를 fragment shader로 출력
//
//void main()
//{
//	gl_Position = vec4(vPos, 1.0);
//	outColor = vColor; //--- vertex data로부터 가져온 컬러 입력을 ourColor에 설정
//}


#version 330 core

layout (location = 0) in vec3 vPos; //--- 응용 프로그램에서 받아온 도형 좌표값
layout (location = 1) in vec3 vColor;

out vec3 outColor;

uniform mat4 modelTransform; //--- 모델링 변환 행렬: uniform 변수로 선언

void main()
{
	gl_Position = modelTransform * vec4(vPos, 1.0); //--- 좌표값에 modelTransform 변환을 적용한다.
	outColor = vColor;
}
