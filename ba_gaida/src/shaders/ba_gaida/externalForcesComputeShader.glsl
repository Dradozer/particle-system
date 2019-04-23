#version 450
layout( local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

layout( shared, binding = 0) coherent buffer
buffer_inPos
{
    vec4 inPos[];
};

layout( shared, binding = 1) coherent buffer
buffer_inVel
{
    vec4 inVel[];
};

uniform float deltaTime;
uniform int particleCount;

void main(void) {
    uint index = gl_GlobalInvocationID.x;
    if(index >= particleCount)
    {
        return;
    } else
    {
        inVel[index].y += -9.81 * deltaTime;
        inPos[index] += inVel[index] *  deltaTime;
    }
}
