#version 430 core

in vec2 UV;

uniform sampler2D texSampler;

out vec4 fragColour;

void main()
{
	vec4 texDiffuseColor = texture2D(texSampler, UV).rgba;
	
	fragColour = texDiffuseColor;
}