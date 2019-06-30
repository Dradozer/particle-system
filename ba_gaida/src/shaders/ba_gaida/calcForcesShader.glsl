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
uniform vec4 externalForce;
uniform vec4 particleSettings;
//float mass;
//float restingDensity;
//float stiffness;
//float radius;

#define kinematicViscosity (0.000001)

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
    vec4 pressure = vec4(0.f);
    vec4 viscosity = vec4(0.f);
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle2[id] = particle1[id];
        neighborGrid = particle1[id].gridID + cubeID(vec4(0,0,0,0));
        for(int j = grid[neighborGrid].currentSortOutPut; j < grid[neighborGrid].currentSortOutPut +  grid[neighborGrid].particlesInGrid; j++){

            const vec4 deltaWeight = deltaW(particle1[id].position,particle1[j].position);

            pressure += ((particle1[id].pressure / pow(particle1[id].density,2)
            + (particle1[j].pressure / pow(particle1[j].density,2)))) * deltaWeight;

            const vec4 distanceVector = (particle1[id].position - particle1[j].position);
            viscosity += (1 / particle1[j].density)
            * (particle1[id].velocity - particle1[j].velocity)
            * ((distanceVector * deltaWeight)
            / (distanceVector *distanceVector + 0.01f * particleSettings.w * particleSettings.w));


        }
        pressure *= (- particleSettings.x /particle1[id].density) * particle1[id].density * particleSettings.x;
        viscosity *= particleSettings.x * kinematicViscosity *  2 * particleSettings.x;

        particle2[id].velocity = particle1[id].velocity + deltaTime * (pressure + viscosity + externalForce) / particleSettings.x;
    }
}