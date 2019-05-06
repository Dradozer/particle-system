#version 450
layout( local_size_x = 16, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
};

layout( std430, binding = 0) coherent buffer buffer_particle
{
    Particle particle[];
};

//layout( std430, binding = 1) coherent buffer buffer_inVelocity
//{
//    vec4 inVelocity[];
//};

uniform float deltaTime;
uniform int particleCount;
#define gravity  (-9.81)

void main(void) {
    uint id = gl_GlobalInvocationID.x;
    if(id >= particleCount)
    {
        return;
    } else
    {
        particle[id].velocity.y += gravity * deltaTime;
        particle[id].position += particle[id].velocity * deltaTime;
    }
}
