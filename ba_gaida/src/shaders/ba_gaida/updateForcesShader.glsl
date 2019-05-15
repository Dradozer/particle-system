#version 450
/*
 * 1.4 ComputeShader
 * Updates the Forces
 */
layout( local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    uint gridID;
    int pad1,pad2,pad3;
};

struct Grid{
    uint id;
    uint count;
    uint lastCount;
    int pad2;
};

layout( std430, binding = 0) writeonly buffer buffer_particle1
{
    Particle particle1[];
};

layout( std430, binding = 1) readonly buffer buffer_particle2
{
    Particle particle2[];
};

layout( std430, binding = 2) coherent buffer buffer_grid
{
    Grid grid[];
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
        particle1[id].position = particle2[id].position +  particle2[id].velocity * deltaTime;
        particle1[id].velocity = particle2[id].velocity;
    }

    if(id >= gridSize.x * gridSize.y *gridSize.z)
    {
        return;
    }else
    {
        grid[id].lastCount = grid[id].count;
        grid[id].count = 0;
    }
}