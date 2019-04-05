#version 430

in layout (location = 0) vec4 position; 
in layout (location = 1) vec4 velocity;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec3 cameraPos;

out vec4 pos;

void main()
{
	vec4 viewPos = viewMatrix * vec4(position.xyz, 1.0);
	gl_Position = projMatrix * viewPos;
	//calc distance
	float dist = length(cameraPos - position.xyz);
	if (dist == 0)
		dist = 0.0000001; //no devision by 0 exeption
	
	gl_PointSize = (175 / dist); //175 size of particle
	pos = position;
}