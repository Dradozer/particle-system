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

float W(vec4 particlePosition ,vec4 neighborPosition){
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

float deltaW(float inPut){
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
    vec4 arbitraryPosition;
    int count = 0;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle1[id] = particle2[id];
        neighborGrid = particle2[id].gridID + cubeID(vec4(0,0,0,0));
 int i = grid[neighborGrid].currentSortOutPut;
        for(int i = grid[neighborGrid].currentSortOutPut; i < grid[neighborGrid].currentSortOutPut +  grid[neighborGrid].particlesInGrid && count < 1; i++){
//            arbitraryPosition += mass * ((particle2[id].arbitraryPosition/ pow(particle2[id].density,2)) + (particle2[i].arbitraryPosition/ pow(particle2[i].density,2))) ;
            arbitraryPosition += mass * ((particle2[id].arbitraryPosition/ pow(particle2[id].density,2) + (particle2[i].arbitraryPosition/ pow(particle2[i].density,2)))) * vec4(deltaW(particle2[id].position.x - particle2[i].position.x), deltaW(particle2[id].position.y - particle2[i].position.y),deltaW(particle2[id].position.z - particle2[i].position.z),0);
            count++;
        }
        particle1[id].velocity = particle2[id].velocity + particle2[id].density * arbitraryPosition * deltaTime * deltaTime;
    }
}