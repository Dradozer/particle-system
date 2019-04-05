#version 430

uniform vec4 color = vec4(1.0f, 0.0f, 0.0f, 1.0f);

out vec4 FragColor;


void main()
{
	FragColor = vec4(color.r, color.g, color.b, color.a);
}