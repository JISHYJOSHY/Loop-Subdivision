#version 430 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec2 UV;

void main()
{
	gl_Position = projMat * viewMat * modelMat * vPosition;
	
	UV = vUV;
}