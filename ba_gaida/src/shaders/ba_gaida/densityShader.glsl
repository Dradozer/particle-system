#version 450
/*
 * 1.5 ComputeShader
 * swaping Particle SSBO's
 */
layout(local_size_x = 100, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    vec4 startPosition;
    vec4 normal;
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

layout(std430, binding = 0) readonly buffer buffer_inParticle
{
    Particle inParticle[];
};

layout(std430, binding = 1) writeonly buffer buffer_outParticle
{
    Particle outParticle[];
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
    float density = 0.1;

    if (id >= particleCount)
    {
        return;
    } else
    {
        outParticle[id] = inParticle[id];
        outParticle[id].density = 0f;
        for (int x = -1; x <= 1; x++){
            for (int y = -1; y <= 1; y++){
                for (int z = -1; z <= 1; z++){

                    neighborGrid = cubeID(inParticle[id].position + vec4(x, y, z, 0));
                    int count = 0;
                    for (int j = grid[neighborGrid].currentSortOutPut; j < grid[neighborGrid].currentSortOutPut +  grid[neighborGrid].particlesInGrid && count <= 16; j++){
                        if(j == id){
                            continue;
                        }
                        density += particleSettings.x * Weight(inParticle[id].position.xyz - inParticle[j].position.xyz);
                        count++;
                    }
                }
            }
        }
        outParticle[id].density = density;
        outParticle[id].pressure = particleSettings.z * (density - particleSettings.y);
    }
}