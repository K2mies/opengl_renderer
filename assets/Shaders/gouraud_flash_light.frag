#version 330 core

//---------------------------------------------------- in/out

in     vec2 TexCoords;

in     vec3 Ambient;
in     vec3 Diffuse;
in     vec3 Specular;

out    vec4 FragColor;

//--------------------------------------------------- structs

struct Material {

       sampler2D diffuse;
       sampler2D specular;
       sampler2D emission;

       float shininess;
};

//------------------------------------------- struct uniforms

uniform Material material;

//--------------------------------------------------------- main

void main()
{
    vec3 diffuseMap   = texture(material.diffuse,  TexCoords).rgb;
    vec3 specularMap  = texture(material.specular, TexCoords).rgb;
    vec3 emissionMap  = texture(material.emission, TexCoords).rgb;


    vec3 ambient      = Ambient   *  diffuseMap;
    vec3 diffuse      = Diffuse   *  diffuseMap;
    vec3 specular     = Specular  *  specularMap;


    vec3 result       = ambient
                      + diffuse
                      + specular;

    FragColor         = vec4(result, 1.0);
}
