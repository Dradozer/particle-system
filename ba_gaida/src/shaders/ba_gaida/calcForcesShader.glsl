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

layout(std430, binding = 0) readonly buffer buffer_particle1
{
    Particle particle1[];
};

layout(std430, binding = 1) writeonly buffer buffer_particle2
{
    Particle particle2[];
};

layout(std430, binding = 2) coherent buffer buffer_grid
{
    Grid grid[];
};

uniform uint particleCount;
uniform float deltaTime;
uniform float buoyCoeff;
uniform ivec4 gridSize;
uniform vec4 externalForce;
uniform vec4 particleSettings;
//float mass;
//float restingDensity;
//float stiffness;
//float radius;

const float kinematicViscosity  = 0.1f;// uniform testen
const float PI = 3.14159265f;

const vec3 upDirection = vec3(0.f, 1.f, 0.f);

float Weight(vec3 relativePosition)
{
    float relativePosition_2 = dot(relativePosition, relativePosition);
    float radius_2 = particleSettings.w * particleSettings.w;

    float temp = 315.0 / (64.0 * PI * pow(particleSettings.w, 9));
    return temp * pow(radius_2 - relativePosition_2, 3) * float(relativePosition_2 <= radius_2);
}

vec3 pressureWeightGradient(vec3 relativePosition)
{
    float c = -45.0 / PI /  pow(particleSettings.w, 6);

    float l = max(length(relativePosition),0.0001);

    return c*relativePosition / l*pow(abs(particleSettings.w - l), 2.0)*float(l <= particleSettings.w);
}

float viscosityLaplaceWeight (vec3 relativePosition)
{
    float radius_2 = particleSettings.w * particleSettings.w;
    float radius_3 = radius_2 * particleSettings.w;
    float radius_6 = radius_3 * radius_3;

    float temp = 45.f /(PI * radius_6);

    float length = length(relativePosition);

    return temp * (particleSettings.w - length) * float(length <= particleSettings.w);
}

uint cubeID(vec4 position){
    return int(floor(position.x) * gridSize.x * gridSize.x + floor(position.y) * gridSize.y + floor(position.z));
}

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    float temperature = particle1[id].density;
    uint neighborGrid;
    vec3 pressure = vec3(0.f);
    vec3 viscosity = vec3(0.f);
    vec3 buoyancy = vec3(0.f);
    if (id >= particleCount)
    {
        return;
    } else
    {
        particle2[id] = particle1[id];
        for (int x = -1; x <= 1; x++){
            for (int y = -1; y <= 1; y++){
                for (int z = -1; z <= 1; z++){

                    neighborGrid = cubeID(particle1[id].position + vec4(x, y, z, 0));
                    int count = 0;
                    for (int j = grid[neighborGrid].currentSortOutPut; j < grid[neighborGrid].currentSortOutPut +  grid[neighborGrid].particlesInGrid && count <= 16; j++){

                        pressure += -1 * particleSettings.x
                        * ((particle1[id].pressure + particle1[j].pressure)/ (2* particle1[j].density))
                        * pressureWeightGradient(particle1[id].position.xyz - particle1[j].position.xyz);

                        viscosity += particleSettings.x
                        * ((particle1[j].velocity.xyz - particle1[id].velocity.xyz)/ particle1[j].density)
                        * viscosityLaplaceWeight(particle1[id].position.xyz - particle1[j].position.xyz);

                        count++;
                    }
                }
            }
        }

        buoyancy = buoyCoeff * temperature * upDirection;
        viscosity *= kinematicViscosity;
        particle2[id].velocity = particle1[id].velocity + vec4(deltaTime * (pressure +  viscosity + buoyancy + externalForce.xyz * particle1[id].density), 0.f);
    }
}