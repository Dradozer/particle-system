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

#define kinematicViscosity (0.000001f)

#define PI 3.14159265f

float Weight(vec3 relativePosition)
{
    float relativePosition_2 = dot(relativePosition, relativePosition);
    float radius_2 = particleSettings.w * particleSettings.w;

    float temp = 315.0 / (64.0 * PI * pow(particleSettings.w, 9));
    return temp * pow(radius_2 - relativePosition_2, 3) * float(relativePosition_2 <= radius_2);
}

vec3 pressureWeight(vec3 relativePosition)
{
    float temp = -45.0 /  (PI * pow(particleSettings.w, 6));

    float length = max(length(relativePosition), 0.0001);

    return temp * relativePosition / length *pow(abs(particleSettings.w - length), 3) * float(length <= particleSettings.w);
}

float viscosityLaplaceWeight (vec3 relativePosition)
{
    float radius_2 = particleSettings.w * particleSettings.w;
    float radius_3 = radius_2 * particleSettings.w;
    float radius_6 = radius_3 * radius_3;

    float temp = 45.f /( PI * radius_6);

    float length = length(relativePosition);

    return temp * (particleSettings.w - length) * float(length <= particleSettings.w);
}

uint cubeID(vec4 position){
    return int(floor(position.x) * gridSize.x * gridSize.x + floor(position.y) * gridSize.y + floor(position.z));
}

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    uint neighborGrid;
    vec3 pressure = vec3(0.f);
    vec3 viscosity = vec3(0.f);
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle2[id] = particle1[id];
        neighborGrid = particle1[id].gridID + cubeID(vec4(0,0,0,0));
        int count = 0;
        for(int j = grid[neighborGrid].currentSortOutPut; j < grid[neighborGrid].currentSortOutPut +  grid[neighborGrid].particlesInGrid && count <= 64; j++){

            pressure += -1 * particleSettings.x
            * ((particle1[id].pressure + particle1[j].pressure)/ 2* particle1[j].density)
            * pressureWeight(particle1[id].position.xyz - particle1[j].position.xyz) ;


            viscosity += particleSettings.x
            * ((particle1[j].velocity.xyz - particle1[id].velocity.xyz)/ particle1[j].density)
            * viscosityLaplaceWeight(particle1[id].position.xyz - particle1[j].position.xyz);


            count++;
        }
        viscosity *= kinematicViscosity;

        particle2[id].velocity = particle1[id].velocity + vec4(deltaTime * deltaTime * (pressure + viscosity + externalForce.xyz * particle1[id].density)/particleSettings.x ,0.f);
    }
}