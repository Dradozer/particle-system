#version 450

in layout (location = 0) vec4 position;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_PointSize = 100;
	gl_Position = projMatrix * viewMatrix * position;
}