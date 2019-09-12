#version 450
/*
 * 1.7 ComputeShader
 * Updates Vel
 */
layout( local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    vec4 startPosition;
    vec4 normal;
    vec4 vorticity;
    float temperature;
    uint memoryPosition;
    float density;
    float pressure;
};

layout( std430, binding = 0) readonly buffer buffer_inParticle
{
    Particle inParticle[];
};

layout( std430, binding = 1) writeonly buffer buffer_outParticle
{
    Particle outParticle[];
};

uniform float deltaTime;
uniform uint particleCount;
uniform ivec4 gridSize;

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    if(id >= particleCount)
    {
        return;
    } else
    {
        outParticle[id] = inParticle[id];
        outParticle[id].position = inParticle[id].position +  inParticle[id].velocity * deltaTime;
    }
}