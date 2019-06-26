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
    float pressure;
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
    float inPut = distance(particlePosition,neighborPosition);
    float weight = 0.f;
    float pi_constant = 3/(2 *3.14159265);
    inPut = 0.5f;
    if(inPut < 1.f){
        weight = pi_constant * ((2.f/3.f) - pow(inPut,2) + 0.5f * pow(inPut,3));
    }else if(inPut < 2.f){
        weight = pi_constant * ((1.f/6.f) * pow(2 - inPut,3));
    }else{
        weight = 0;
    }
    return weight / pow(radius,3);
}

vec4 deltaW(vec4 particlePosition ,vec4 neighborPosition){
    float weight = W(particlePosition.xyz, neighborPosition.xyz);
    return vec4( weight / particlePosition.x, weight / particlePosition.y, weight / particlePosition.z ,0.f);
}

uint cubeID(vec4 position){
    return int(floor(position.x) * gridSize.x * gridSize.x + floor(position.y) * gridSize.y + floor(position.z));
}

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    uint neighborGrid;
    float mass = 1f;
    vec4 viscosity = vec4(0.f);
    float radius = 1.f;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle2[id] = particle1[id];
        neighborGrid = particle1[id].gridID + cubeID(vec4(0,0,0,0));
        for(int i = grid[neighborGrid].currentSortOutPut; i < grid[neighborGrid].currentSortOutPut +  grid[neighborGrid].particlesInGrid; i++){
            viscosity += (mass / particle1[i].density) * (particle1[id].arbitraryPosition - particle1[i].arbitraryPosition) * (((particle1[id].position - particle1[i].position) * deltaW(particle1[id].position,particle1[i].position)) / ((particle1[id].position - particle1[i].position) *(particle1[id].position - particle1[i].position) + 0.01f * radius * radius));
        }
        particle2[id].velocity = particle1[id].velocity + 2 * viscosity * deltaTime;
    }
}