#version 450
/*
 * 1.5 ComputeShader
 * swaping Particle SSBO's
 */
layout( local_size_x = 100, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    vec4 startPosition;
    float temperature;
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
uniform vec4 externalForce;
uniform vec4 particleSettings;
//float mass;
//float restingDensity;
//float stiffness;
//float radius;

float W(vec3 particlePosition ,vec3 neighborPosition){
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
    return weight / pow(particleSettings.w,3);
}

uint cubeID(vec4 position){
    return int(floor(position.x) * gridSize.x * gridSize.x + floor(position.y) * gridSize.y + floor(position.z));
}

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    uint neighborGrid;
    vec3 arbitraryPosition;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle1[id] = particle2[id];

//        for(int i = grid[neighborGrid].currentSortOutPut; i < grid[neighborGrid].currentSortOutPut +  grid[neighborGrid].particlesInGrid; i++){
//            arbitraryPosition += (particleSettings.x / particle2[i].density) * particle2[i].arbitraryPosition.xyz * W(particle2[id].position.xyz, particle2[i].position.xyz);
//        }
//        particle1[id].arbitraryPosition.xyz = arbitraryPosition;
    }
}