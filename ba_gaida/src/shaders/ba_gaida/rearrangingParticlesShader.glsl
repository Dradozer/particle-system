#version 450
/*
 * 1.3 ComputeShader
 * rearranging Particle
 */
layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    vec4 arbitraryPosition;
    uint gridID;
    uint memoryPosition;
    float density;
    float pressureGradient;
};

struct Grid{
    int gridID;
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

int cubeid;

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    if (id >= particleCount)
    {
        return;
    } else
    {
        particle1[grid[particle2[id].gridID].currentSortOutPut + particle2[id].memoryPosition] = particle2[id];
    }
}