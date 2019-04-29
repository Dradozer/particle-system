#version 450

out vec4 FragColor;

in vec4 pos;
uniform vec3 cameraPos;
#define red 179
#define green 20
#define blue 20

void main()
{
    //the closer the darker and the further away the brighter the particle are
//    float dist = length(cameraPos - pos.xyz) / 12.5;
    //change color depending on distance
    float r = red / 255.f;
    float g = green / 255.f;
    float b = blue / 255.f;

    FragColor = vec4(r, g, b, 1.0f);
}