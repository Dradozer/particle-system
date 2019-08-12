#version 450
/*
 * -1.0 ComputeShader
 * reset all
 */
layout( local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    vec4 startPosition;
    vec4 normal;
    float temperature;
    uint memoryPosition;
    float density;
    float pressure;
};

layout( std430, binding = 0) writeonly buffer buffer_outParticle
{
    Particle outParticle[];
};

layout( std430, binding = 1) readonly buffer buffer_inParticle
{
    Particle inParticle[];
};

uniform uint particleCount;

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    if(id >= particleCount)
    {
        return;
    } else
    {
        outParticle[id].position = vec4(inParticle[id].startPosition.xyz,0.f);
        outParticle[id].velocity = vec4(0.f,0.f,0.f,0.f);
        outParticle[id].temperature = inParticle[id].startPosition.w;
    }
}