#version 450
/*
 * 1.5 ComputeShader
 * swaping Particle SSBO's
 */
layout( local_size_x = 100, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    uint gridID;
    uint memoryPosition;
    float density;
    uint pad3;
};

struct Grid{
    int gridID;
    int particlesInGrid;
    int previousSortOutPut;
    int currentSortOutPut;
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

uniform uint particleCount;
uniform float deltaTime;
uniform ivec4 gridSize;

uint cubeID(vec4 position){
    return int(floor(position.x) * gridSize.x * gridSize.x + floor(position.y) * gridSize.y + floor(position.z));
}

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    uint tempID;
    uint neighborGrid;
    float mass = 0.1f;
    vec4 pressureSum;
    vec4 pressureForce;
    int count;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle2[id] = particle1[id];

        neighborGrid = particle1[id].gridID + cubeID(vec4(0,0,0,0));

        for(int i = grid[neighborGrid].currentSortOutPut; i < grid[neighborGrid].currentSortOutPut +  grid[neighborGrid].particlesInGrid && count < 1024; i++){
            pressureSum +=  mass *((particle1[id].density)/pow(particle1[id].density,2) + (particle1[i].density)/pow(particle1[i].density,2)) * (particle1[id].position - particle1[i].position);
            count++;
        }

        pressureForce = - mass * pressureSum / grid[neighborGrid].particlesInGrid;

        particle2[id].velocity = particle1[id].velocity + pressureForce * deltaTime;
    }
}