#version 430

in layout (location = 0) vec3 position;
in layout (location = 1) vec3 normal;
in layout (location = 2) vec4 modelMatrixCube;

uniform mat4 projViewMatrix;
uniform mat4 modelMatrix;

uniform bool drawParticle = false;
uniform bool drawCube = false;
uniform bool drawCubeInstanced = false;

void main()
{
	mat4 localModelMatrix = modelMatrix;
	
	if (drawCube && drawCubeInstanced) {
		localModelMatrix = mat4(1.0f);
		localModelMatrix[0][0] *= modelMatrixCube.w;
		localModelMatrix[1][1] *= modelMatrixCube.w;
		localModelMatrix[2][2] *= modelMatrixCube.w;
		localModelMatrix[3][0] = modelMatrixCube.x;
		localModelMatrix[3][1] = modelMatrixCube.y;
		localModelMatrix[3][2] = modelMatrixCube.z;
	}
	gl_Position = projViewMatrix * localModelMatrix * vec4(position, 1.0f);
	
	if (drawParticle) {
		gl_PointSize = 3;
	}
}