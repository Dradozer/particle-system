#version 450
/*
 * -1.0 ComputeShader
 * reset all
 */
layout( local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    vec4 arbitraryPosition;
    float temperature;
    uint memoryPosition;
    float density;
    float pressure;
};

layout( std430, binding = 0) writeonly buffer buffer_particle1
{
    Particle particle1[];
};

layout( std430, binding = 1) readonly buffer buffer_particle2
{
    Particle particle2[];
};

uniform uint particleCount;

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle1[id].position = particle2[id].arbitraryPosition;
        particle1[id].velocity = vec4(0.f,0.f,0.f,0.f);
    }
}