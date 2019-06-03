#version 450
/*
 * 2.2 FragmentShader
 */
out vec4 FragColor;

in vec4 pos;
in vec4 vel;
uniform vec3 cameraPos;
#define red 179
#define green 20
#define blue 20

void main()
{
    //the closer the darker and the further away the brighter the particle are
//    float dist = length(cameraPos - pos.xyz) / 12.5;
    //change color depending on distance
//    float r = red / 255.f;
//    float g = green / 255.f;
//    float b = blue / 255.f;
    vec4 posNormal = normalize(pos);
    FragColor = vec4 (posNormal.xyz,1.f);
}