#version 450
/*
 * 1.1 ComputeShader
 * Lable Particle
 */
layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    uint gridID;
    uint pad1, pad2, pad3;
};

struct Grid{
    int id;
    int particlesInGrid;
    int previousSortOutPut;
    int currentSortOutPut;
};

layout(std430, binding = 0) readonly buffer buffer_particle1
{
    Particle particle1[];
};

layout(std430, binding = 1) writeonly buffer buffer_particle2
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
    uint temp;
    if (id >= particleCount)
    {
        return;
    } else
    {
        particle2[id] = particle1[id];
        cubeid = int(floor(particle1[id].position.x) * gridSize.x * gridSize.x + floor(particle1[id].position.y) * gridSize.y + floor(particle1[id].position.z));
        particle2[id].gridID = cubeid;
        memoryBarrierBuffer();
        temp = atomicAdd(grid[cubeid].particlesInGrid, 1);
        particle2[id].pad1 = temp;
        memoryBarrier();
    }
}