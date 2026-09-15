#version 330 core

//---------------------------------------------------- input

in vec2 TexCoords;

//--------------------------------------------------- output

out vec4 FragColor;

//------------------------------------------------- material

struct Material
{
    sampler2D texture_diffuse1;
};

uniform Material material;

//------------------------------------------------------- main

void main()
{
    FragColor = texture(
        material.texture_diffuse1,
        TexCoords
    );
}
