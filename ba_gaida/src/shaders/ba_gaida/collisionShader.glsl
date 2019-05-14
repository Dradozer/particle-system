#version 450
/*
 * 1.3 ComputeShader
 * Calculates the Collision
 */
layout( local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    uint gridID;
    int pad1,pad2,pad3;
};

layout( std430, binding = 0) readonly buffer buffer_particle1
{
    Particle particle1[];
};

layout( std430, binding = 1) writeonly buffer buffer_particle2
{
    Particle particle2[];
};

uniform float deltaTime;
uniform uint particleCount;
uniform ivec4 gridSize;

uint lowBoundary = 1;

#define energyloss 0.6f

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle2[id] = particle1[id];
        //collisition X-Axis
        if(particle1[id].position.x > gridSize.x)
        {
         particle2[id].position.x = gridSize.x;
         particle2[id].velocity = vec4(reflect(particle1[id].velocity.xyz,vec3(-1.f,0.f,0.f)),1.f) * energyloss;
        }else if(particle1[id].position.x < lowBoundary)
        {
            particle2[id].position.x = lowBoundary;
            particle2[id].velocity = vec4(reflect(particle1[id].velocity.xyz,vec3(1.f,0.f,0.f)),1.f) * energyloss;
        }
        //collisition Y-Axis
        if(particle1[id].position.y > gridSize.y)
        {
            particle2[id].position.y = gridSize.y;
            particle2[id].velocity = vec4(reflect(particle1[id].velocity.xyz,vec3(0.f,-1.f,0.f)),1.f) * energyloss;
        }else if(particle1[id].position.y < lowBoundary)
        {
            particle2[id].position.y = lowBoundary;
            particle2[id].velocity = vec4(reflect(particle1[id].velocity.xyz,vec3(0.f,1.f,0.f)),1.f) * energyloss;
        }
        //collisition Z-Axis
        if(particle1[id].position.z > gridSize.z)
        {
            particle2[id].position.z = gridSize.z;
            particle2[id].velocity = vec4(reflect(particle1[id].velocity.xyz,vec3(0.f,0.f,-1.f)),1.f) * energyloss;
        }else if(particle1[id].position.z < lowBoundary)
        {
            particle2[id].position.z = lowBoundary;
            particle2[id].velocity = vec4(reflect(particle1[id].velocity.xyz,vec3(0.f,0.f,1.f)),1.f) * energyloss;
        }
    }
}