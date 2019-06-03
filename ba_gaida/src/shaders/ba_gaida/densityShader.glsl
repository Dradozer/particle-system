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
    float density;
    float restPressure = 0.5f;
    float mass = 0.5f;
    float stiffnes1 = - 0.5f;
    float stiffnes2 = 0.1f;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle1[id] = particle2[id];

        neighborGrid = particle2[id].gridID + cubeID(vec4(0,0,0,0));
        for(int i = grid[neighborGrid].currentSortOutPut; i < grid[neighborGrid].currentSortOutPut +  grid[neighborGrid].particlesInGrid; i++){
            density +=  distance(particle2[id].position,particle2[i].position)* mass;
        }
        particle1[id].density = ((stiffnes1 * restPressure)/ stiffnes2) * (pow((density/restPressure),stiffnes2)-1);;
    }
}