#version 450
/*
 * 2.2 FragmentShader
 */
out vec4 FragColor;

in vec4 pos;
in vec4 vel;
uniform vec3 cameraPos;

void main()
{
    vec4 posNormal = normalize(pos + vec4(0.5f,0.5f,0.5f,0.f));
//    FragColor = vec4 (posNormal.xyz,1.f);
    FragColor = vec4(1.f) - vec4 (posNormal.xyz,0.f);
}