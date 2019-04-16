#version 450

out vec4 FragColor;

in vec4 pos;
uniform vec3 cameraPos;
//uniform float red;
//uniform float green;
//uniform float blue;

void main()
{
    //the closer the darker and the further away the brighter the particle are
    float dist = length(cameraPos - pos.xyz) / 12.5;
    //change color depending on distance
    float r = 1- (255 * dist / 255);
    float g = 0 * dist / 255;
    float b = 0 * dist / 255;

    FragColor = vec4(r, g, b, 1.0f);
}