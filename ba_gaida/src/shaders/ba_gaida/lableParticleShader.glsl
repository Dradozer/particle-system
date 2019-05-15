#version 450
/*
 * 1.1 ComputeShader
 * Lable Particle
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

layout( std430, binding = 0) readonly buffer buffer_particle1
{
    Particle particle1[];
};

layout( std430, binding = 1) writeonly buffer buffer_particle2
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

int cubeid;

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle2[id] = particle1[id];
        cubeid = int(particle1[id].position.x * gridSize.x * gridSize.x + particle1[id].position.y * gridSize.y + particle1[id].position.z);
        particle2[id].gridID = cubeid;
        atomicAdd(grid[cubeid].count,1);
    }
}