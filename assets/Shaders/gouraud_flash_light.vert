#version 330 core

//------------------------------------------------ attributes

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//---------------------------------------------------- in/out

out vec2 TexCoords;

out vec3 Ambient;
out vec3 Diffuse;
out vec3 Specular;

//-------------------------------------------------- uniforms

uniform vec3 viewPosition;

//--------------------------------------------------- structs

struct Pass {

    float strength;
    float intensity;
    float shininess;
    vec3  color;
};

struct Spacial {

    vec4  position;
    vec4  direction;

    vec3  ambient;
    vec3  diffuse;
    vec3  specular;

    float constant;
    float linear;
    float quadratic;
    float cutoff;
};

struct Material {

    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;

    float shininess;
};

struct Light {

    vec4  position;
    vec4  direction;

    vec3  ambient;
    vec3  diffuse;
    vec3  specular;

    float constant;
    float linear;
    float quadratic;
    float cutoff;
};

//------------------------------------------------- matrices

struct Matrix {

    mat4 local;
    mat4 model;
    mat4 view;
    mat4 projection;

    mat3 normal;
};

//------------------------------------------- struct uniforms

uniform Matrix   matrix;
uniform Material material;
uniform Light    lighting;

//--------------------------------------------------------- main

void main()
{
    mat4 clip_space = matrix.projection
                    * matrix.view
                    * matrix.model
                    * matrix.local;

    gl_Position = clip_space * vec4(aPos, 1.0);

    TexCoords = aTexCoords;

    // Gouraud lighting
    // ----------------------------------------------------------

    vec3      normal = normalize(matrix.normal * aNormal);

    Spacial   fragment;
              fragment.position     = matrix.model * vec4(aPos, 1.0);


    Spacial   light;
              light.position        = lighting.position;
              light.direction.xyz   = light.position.xyz - fragment.position.xyz;
              light.direction.xyz   = normalize(light.direction.xyz);
              light.direction       = vec4(light.direction.xyz, 0.0);

              light.ambient         = lighting.ambient;
              light.diffuse         = lighting.diffuse;
              light.specular        = lighting.specular;

              light.constant        = lighting.constant;
              light.linear          = lighting.linear;
              light.quadratic       = lighting.quadratic;
              light.cutoff          = lighting.cutoff;


    Spacial   view;
              view.position         = vec4(viewPosition, 1.0);
              view.direction.xyz    = view.position.xyz - fragment.position.xyz;
              view.direction.xyz    = normalize(view.direction.xyz);
              view.direction        = vec4(view.direction.xyz, 0.0);


    Spacial   reflection;
              reflection.direction.xyz  = reflect(-light.direction.xyz, normal);
              reflection.direction      = vec4(reflection.direction.xyz, 0.0);


    // attenuation
    // ----------------------------------------------------------

    float     distance;
              distance                 = length(light.position.xyz - fragment.position.xyz);

    float     attenuation;
              attenuation              = light.constant
                                       + light.linear * distance
                                       + light.quadratic * (distance * distance);

              attenuation              = 1.0 / attenuation;


    // spotlight
    // ----------------------------------------------------------

    float     theta;
              theta = dot(light.direction.xyz, normalize(-lighting.direction.xyz));


    // Ambient
    // ----------------------------------------------------------

    Pass       ambient;
               ambient.color = light.ambient;


    // Diffuse
    // ----------------------------------------------------------

    Pass       diffuse;
               diffuse.intensity = dot(normal, light.direction.xyz);
               diffuse.intensity = max(diffuse.intensity, 0.0);

               diffuse.color     = light.diffuse * diffuse.intensity;


    // Specular
    // ----------------------------------------------------------

    Pass specular;
         specular.intensity = dot(view.direction.xyz, reflection.direction.xyz);
         specular.intensity = max(specular.intensity, 0.0);
         specular.intensity = pow(specular.intensity, material.shininess);

         specular.color     = light.specular * specular.intensity;


    // Spotlight output
    // ----------------------------------------------------------

    if (theta > light.cutoff)
    {
        Ambient  = ambient.color  * attenuation;
        Diffuse  = diffuse.color  * attenuation;
        Specular = specular.color * attenuation;
    }
    else
    {
        Ambient  = ambient.color;
        Diffuse  = vec3(0.0);
        Specular = vec3(0.0);
    }
}
