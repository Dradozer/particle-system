#version 450
/*
 * 1.5 ComputeShader
 * swaping Particle SSBO's
 */
layout( local_size_x = 100, local_size_y = 1, local_size_z = 1) in;

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

float W(vec3 particlePosition ,vec3 neighborPosition){
    float radius = 1.f;
    float inPut = length(particlePosition - neighborPosition)/radius;
    float pi_constant = 3/(2 *3.14159265);
    if(inPut < 1){
        return pi_constant * (2/3 - pow(inPut,2) + 0.5f * pow(inPut,3));
    }else if(inPut < 2){
        return pi_constant * (1/6 * pow(2- inPut,3));
    }else{
        return 0;
    }
}

uint cubeID(vec4 position){
    return int(floor(position.x) * gridSize.x * gridSize.x + floor(position.y) * gridSize.y + floor(position.z));
}

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    uint neighborGrid;
    float mass = 0.1f;
    vec3 arbitraryPosition;
    int count = 0;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle2[id] = particle1[id];
        neighborGrid = particle1[id].gridID + cubeID(vec4(0,0,0,0));

        for(int i = grid[neighborGrid].currentSortOutPut; i < grid[neighborGrid].currentSortOutPut +  grid[neighborGrid].particlesInGrid; i++){
            arbitraryPosition += (mass / particle1[id].density) * particle1[id].arbitraryPosition.xyz * W(particle1[id].position.xyz, particle1[i].position.xyz);
        }
        particle2[id].arbitraryPosition.xyz = arbitraryPosition;
    }
}