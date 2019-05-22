#version 450
/*
 * 1.5 ComputeShader
 * Updates the Forces
 */
layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    uint gridID;
    uint memoryPosition;
    uint pad2, pad3;
};

struct Grid{
    int id;
    int particlesInGrid;
    int previousSortOutPut;
    int currentSortOutPut;
};

layout(std430, binding = 0) writeonly buffer buffer_particle1
{
    Particle particle1[];
};

layout(std430, binding = 1) readonly buffer buffer_particle2
{
    Particle particle2[];
};

layout(std430, binding = 2) coherent buffer buffer_grid
{
    Grid grid[];
};

uniform float deltaTime;
uniform uint particleCount;
uniform ivec4 gridSize;

uint maxCubes = (gridSize.x * gridSize.y * gridSize.z);

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    if (id >= particleCount)
    {
        return;
    } else
    {
        particle1[id].position = particle2[id].position +  particle2[id].velocity * deltaTime;
        particle1[id].velocity = particle2[id].velocity;
    }
}