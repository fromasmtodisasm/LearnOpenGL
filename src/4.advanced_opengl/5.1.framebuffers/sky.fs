#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    FragColor = vec4(mix(vec3(1,1,1), vec3(0.5,0.7,1), TexCoords.y), 1);
} 

