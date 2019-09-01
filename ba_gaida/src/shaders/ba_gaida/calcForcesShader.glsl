#version 450
/*
 * 1.5 ComputeShader
 * calc Particle Forces
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
uniform float buoyCoeff;
uniform ivec4 gridSize;
uniform vec4 externalForce;
uniform vec4 particleSettings;
uniform float heatFlow;
//float mass;
//float restingDensity;
//float stiffness;
//float radius;

const float kinematicViscosity  = 0.25f;// uniform testen
const float PI = 3.14159265f;

const vec3 upDirection = vec3(0.f, 1.f, 0.f);

float Weight(vec3 relativePosition)
{
    float relativePosition_2 = dot(relativePosition, relativePosition);
    float radius_2 = particleSettings.w * particleSettings.w;

    float temp = 315.0 / (64.0 * PI * pow(particleSettings.w, 9));
    return temp * pow(radius_2 - relativePosition_2, 3) * float(relativePosition_2 <= radius_2);
}

vec3 gradientWeight(vec3 relativePosition)
{
    float c = -45.0 / PI /  pow(particleSettings.w, 6);

    float l = max(length(relativePosition), 0.0001);

    return c*relativePosition / l*pow(abs(particleSettings.w - l), 2.0)*float(l <= particleSettings.w);
}

float laplaceWeight (vec3 relativePosition)
{
    float radius_2 = particleSettings.w * particleSettings.w;
    float radius_3 = radius_2 * particleSettings.w;
    float radius_6 = radius_3 * radius_3;

    float temp = 45.f /(PI * radius_6);

    float length = length(relativePosition);

    return temp * (particleSettings.w - length) * float(length <= particleSettings.w);
}

float thermalCon (float temperatureDiff){
    return heatFlow /(particleSettings.w * temperatureDiff);
}

uint cubeID(vec4 position){
    return int(floor(position.x) * gridSize.x * gridSize.x + floor(position.y) * gridSize.y + floor(position.z));
}

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    float temperature = inParticle[id].temperature;
    uint neighborGrid;
    vec3 pressure = vec3(0.f);
    vec3 viscosity = vec3(0.f);
    vec3 buoyancy = vec3(0.f);
    vec3 vorticity = vec3(0.f);

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
                        if (j == id){
                            continue;
                        }
                        pressure += -1 * particleSettings.x
                        * ((inParticle[id].pressure + inParticle[j].pressure)/ (2* inParticle[j].density))
                        * gradientWeight(inParticle[id].position.xyz - inParticle[j].position.xyz);

                        viscosity += particleSettings.x
                        * ((inParticle[j].velocity.xyz - inParticle[id].velocity.xyz)/ inParticle[j].density)
                        * laplaceWeight(inParticle[id].position.xyz - inParticle[j].position.xyz);

                        vorticity += (cross(inParticle[j].vorticity.xyz, inParticle[id].position.xyz - inParticle[j].position.xyz))
                        * Weight(inParticle[id].position.xyz - inParticle[j].position.xyz);

                        if (inParticle[id].density > 28){
                            temperature += ((particleSettings.x / (inParticle[id].density * inParticle[j].density))
                            * heatFlow
                            * (dot((inParticle[id].position.xyz - inParticle[j].position.xyz), gradientWeight(inParticle[id].position.xyz - inParticle[j].position.xyz))
                            /(dot((inParticle[id].position.xyz - inParticle[j].position.xyz), (inParticle[id].position.xyz - inParticle[j].position.xyz)) + 0.0001f * 0.0001f)))
                            * deltaTime;
                        }

                        count++;
                    }
                }
            }
        }

        if (length(inParticle[id].normal) >= 2){
            temperature -= (inParticle[id].temperature / 50) * deltaTime;
        }

        outParticle[id].temperature = temperature;
//        buoyancy = buoyCoeff * 10.5 * upDirection * inParticle[id].density;
        buoyancy = buoyCoeff * temperature * upDirection * inParticle[id].density;
        viscosity *= kinematicViscosity;
        outParticle[id].velocity = inParticle[id].velocity + vec4(deltaTime * ( vorticity + pressure +  viscosity + (buoyancy + externalForce.xyz * inParticle[id].density)* 0.1), 0.f);
    }
}