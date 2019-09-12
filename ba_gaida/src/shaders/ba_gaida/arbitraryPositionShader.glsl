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
    vec4 vorticity;
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

layout(std430, binding = 0) writeonly buffer buffer_outParticle
{
    Particle outParticle[];
};

layout(std430, binding = 1) readonly buffer buffer_inParticle
{
    Particle inParticle[];
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
uniform float temperature;
//float mass;
//float restingDensity;
//float stiffness;
//float radius;

const float PI = 3.14159265f;
const float beta = 5; // 0-1

vec3 gradientWeight(vec3 relativePosition)
{
    float c = -45.0 / PI /  pow(particleSettings.w, 6);

    float l = max(length(relativePosition), 0.0001);

    return c * relativePosition / l * pow(abs(particleSettings.w - l), 2.0)*float(l <= particleSettings.w);
}

uint cubeID(vec4 position){
    return int(floor(position.x) * gridSize.x * gridSize.x + floor(position.y) * gridSize.y + floor(position.z));
}

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    uint neighborGrid;
    vec3 normal = vec3(0.f);
    vec3 k = vec3(0.f);
    if (id >= particleCount)
    {
        return;
    } else
    {
        outParticle[id] = inParticle[id];

        for (int x = -1; x <= 1; x++){
            for (int y = -1; y <= 1; y++){
                for (int z = -1; z <= 1; z++){
                    neighborGrid = cubeID(inParticle[id].position + vec4(x, y, z, 0));
                    int count = 0;
                    for (int j = grid[neighborGrid].currentSortOutPut; j < grid[neighborGrid].currentSortOutPut +  grid[neighborGrid].particlesInGrid && count <= 16; j++){
                        if(j == id){
                            continue;
                        }
                        normal += (particleSettings.x / inParticle[j].density)* gradientWeight(inParticle[id].position.xyz - inParticle[j].position.xyz);
                        count++;
                    }
                }
            }

        }
        outParticle[id].normal = vec4(normal, 0.f);

        k = inParticle[id].vorticity.xyz * gradientWeight(inParticle[id].velocity.xyz);

        outParticle[id].vorticity = vec4((k + beta * cross(normal, externalForce.xyz)),0.f) * deltaTime;

    }
}