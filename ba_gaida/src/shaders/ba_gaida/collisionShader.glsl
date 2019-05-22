#version 450
/*
 * 1.7 ComputeShader
 * Calculates the Collision
 */
layout( local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    uint gridID;
    uint memoryPosition;
    uint pad2, pad3;
};

layout( std430, binding = 0) writeonly buffer buffer_particle1
{
    Particle particle1[];
};

layout( std430, binding = 1) readonly buffer buffer_particle2
{
    Particle particle2[];
};

uniform float deltaTime;
uniform uint particleCount;
uniform ivec4 gridSize;
uniform ivec4 origin = ivec4(0);

#define energyloss 0.6f

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle1[id] = particle2[id];
        //collisition X-Axis
        if(particle2[id].position.x >= gridSize.x + origin.x)
        {
         particle1[id].position.x = gridSize.x  + origin.x - 0.001f;
         particle1[id].velocity = vec4(reflect(particle2[id].velocity.xyz,vec3(-1.f,0.f,0.f)),1.f) * energyloss;
        }else if(particle2[id].position.x < origin.x)
        {
            particle1[id].position.x = origin.x;
            particle1[id].velocity = vec4(reflect(particle2[id].velocity.xyz,vec3(1.f,0.f,0.f)),1.f) * energyloss;
        }
        //collisition Y-Axis
        if(particle2[id].position.y >= gridSize.y + origin.y)
        {
            particle1[id].position.y = gridSize.y + origin.y - 0.001f;
            particle1[id].velocity = vec4(reflect(particle2[id].velocity.xyz,vec3(0.f,-1.f,0.f)),1.f) * energyloss;
        }else if(particle2[id].position.y < origin.y)
        {
            particle1[id].position.y = origin.y;
            particle1[id].velocity = vec4(reflect(particle2[id].velocity.xyz,vec3(0.f,1.f,0.f)),1.f) * energyloss;
        }
        //collisition Z-Axis
        if(particle2[id].position.z >= gridSize.z + origin.z)
        {
            particle1[id].position.z = gridSize.z + origin.z - 0.001f;
            particle1[id].velocity = vec4(reflect(particle2[id].velocity.xyz,vec3(0.f,0.f,-1.f)),1.f) * energyloss;
        }else if(particle2[id].position.z < origin.y)
        {
            particle1[id].position.z = origin.y;
            particle1[id].velocity = vec4(reflect(particle2[id].velocity.xyz,vec3(0.f,0.f,1.f)),1.f) * energyloss;
        }
    }
}