#version 450
/*
 * 1.5 ComputeShader
 * swaping Particle SSBO's
 */
layout( local_size_x = 100, local_size_y = 1, local_size_z = 1) in;

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

struct Grid{
    int gridID;
    int particlesInGrid;
    int previousSortOutPut;
    int currentSortOutPut;
};

layout( std430, binding = 0) writeonly buffer buffer_outParticle
{
    Particle outParticle[];
};

layout( std430, binding = 1) readonly buffer buffer_inParticle
{
    Particle inParticle[];
};

layout( std430, binding = 2) coherent buffer buffer_grid
{
    Grid grid[];
};

uniform uint particleCount;
uniform ivec4 gridSize;

int cubeid;

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    uint tempID;
    if(id >= particleCount)
    {
        return;
    } else
    {
        outParticle[id] = inParticle[id];
    }
}