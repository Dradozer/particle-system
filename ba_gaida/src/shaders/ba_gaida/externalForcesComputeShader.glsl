#version 450
layout( local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

layout( std430, binding = 0) coherent buffer
buffer_inPosition
{
    vec4 inPosition[];
};

layout( std430, binding = 1) coherent buffer
buffer_inVelocity
{
    vec4 inVelocity[];
};

uniform float deltaTime;
uniform int particleCount;
#define gravity  (-9.81/10)

void main(void) {
    uint index = gl_GlobalInvocationID.x;
    if(index >= particleCount)
    {
        return;
    } else
    {
        inVelocity[index].y += gravity * deltaTime;
        inPosition[index] += inVelocity[index] *  deltaTime;
    }
}
