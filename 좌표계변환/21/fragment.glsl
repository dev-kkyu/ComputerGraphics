#version 330 core

out vec4 FragColor;
in vec3 outColor;

uniform int option;
uniform vec3 Color;

void main()
{
	if(option == 1)
		FragColor = vec4(Color, 1.0);
	else
		FragColor = vec4(outColor, 1.0);
}