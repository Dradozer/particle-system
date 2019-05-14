#version 450
/*
 * 1.2 ComputeShader
 * Calculates the Gravity
 */
layout( local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    uint gridID;
    int pad1,pad2,pad3;
};

layout( std430, binding = 0) writeonly buffer buffer_particle1
{
    Particle particle1[];
};

layout( std430, binding = 1) readonly buffer buffer_particle2
{
    Particle particle2[];
};

uniform float deltaTime;
uniform uint particleCount;
uniform ivec4 gridSize;

#define gravity  (-9.81)

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle1[id]= particle2[id];
        particle1[id].velocity.y = particle2[id].velocity.y +  gravity * deltaTime;
    }
}