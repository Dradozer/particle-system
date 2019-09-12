#version 450
/*
 * 1.7 ComputeShader
 * Updates Vel and calculates the Collision
 */
layout( local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

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

layout( std430, binding = 0) writeonly buffer buffer_outParticle
{
    Particle outParticle[];
};

layout( std430, binding = 1) readonly buffer buffer_inParticle
{
    Particle inParticle[];
};

uniform float deltaTime;
uniform uint particleCount;
uniform ivec4 gridSize;
uniform ivec4 origin = ivec4(1);

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    const float energyloss = 0.2;
    if(id >= particleCount)
    {
        return;
    } else
    {
        outParticle[id] = inParticle[id];
        //collisition X-Axis
        if(inParticle[id].position.x >= gridSize.x - origin.x)
        {
            outParticle[id].position.x = gridSize.x  - origin.x - 0.001f;
            outParticle[id].velocity = vec4(reflect(inParticle[id].velocity.xyz,vec3(-1.f,0.f,0.f)),1.f) * energyloss;
        }else if(inParticle[id].position.x < origin.x)
        {
            outParticle[id].position.x = origin.x + 0.001f;
            outParticle[id].velocity = vec4(reflect(inParticle[id].velocity.xyz,vec3(1.f,0.f,0.f)),1.f) * energyloss;
        }
        //collisition Y-Axis
        if(inParticle[id].position.y >= gridSize.y - origin.y)
        {
            outParticle[id].position.y = gridSize.y - origin.y - 0.001f;
            outParticle[id].velocity = vec4(reflect(inParticle[id].velocity.xyz,vec3(0.f,-1.f,0.f)),1.f) * energyloss;
        }else if(inParticle[id].position.y < origin.y)
        {
            outParticle[id].position.y = origin.y + 0.001f;
            outParticle[id].velocity = vec4(reflect(inParticle[id].velocity.xyz,vec3(0.f,1.f,0.f)),1.f) * energyloss;
        }
        //collisition Z-Axis
        if(inParticle[id].position.z >= gridSize.z - origin.z)
        {
            outParticle[id].position.z = gridSize.z - origin.z - 0.001f;
            outParticle[id].velocity = vec4(reflect(inParticle[id].velocity.xyz,vec3(0.f,0.f,-1.f)),1.f) * energyloss;
        }else if(inParticle[id].position.z < origin.z)
        {
            outParticle[id].position.z = origin.z + 0.001f;
            outParticle[id].velocity = vec4(reflect(inParticle[id].velocity.xyz,vec3(0.f,0.f,1.f)),1.f) * energyloss;
        }
    }
}