#version 450
/*
 * 1.5 ComputeShader
 * swaping Particle SSBO's
 */
layout(local_size_x = 100, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    vec4 arbitraryPosition;
    float temperature;
    uint memoryPosition;
    float density;
    float pressure;
};

struct Grid{
    uint id;
    uint particlesInGrid;
    uint particleToUse;
    uint currentSortOutPut;
    uint particles [16];
};

layout(std430, binding = 0) writeonly buffer buffer_particle1
{
    Particle particle1[];
};

layout(std430, binding = 1) readonly buffer buffer_particle2
{
    Particle particle2[];
};

layout(std430, binding = 2) coherent buffer buffer_grid
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

const float PI = 3.14159265f;

uint cubeID(vec4 position){
    return int(floor(position.x) * gridSize.x * gridSize.x + floor(position.y) * gridSize.y + floor(position.z));
}

float Weight(vec3 relativePosition)
{
    float relativePosition_2 = dot(relativePosition, relativePosition);
    float radius_2 = particleSettings.w * particleSettings.w;

    float temp = 315.0 / (64.0 * PI * pow(particleSettings.w, 9));
    return temp * pow(radius_2 - relativePosition_2, 3) * float(relativePosition_2 <= radius_2);
}

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    uint neighborGrid;
    float density = 0;
    float dc = 1;

    if (id >= particleCount)
    {
        return;
    } else
    {
        particle1[id] = particle2[id];
        particle1[id].density = 0f;
        particle1[id].velocity.w = 0;
        for (int x = -1; x <= 1; x++){
            for (int y = -1; y <= 1; y++){
                for (int z = -1; z <= 1; z++){
                    neighborGrid = cubeID(particle2[id].position + vec4(x, y, z, 0));
                    for (int j = 0; j < grid[neighborGrid].particleToUse; j++){
                        density += particleSettings.x
                        * Weight(particle2[id].position.xyz - particle2[grid[neighborGrid].particles[j]].position.xyz);
                    }
                }
            }
        }
        particle1[id].density = density;
        particle1[id].pressure = particleSettings.z * (density - particleSettings.y);
    }
}