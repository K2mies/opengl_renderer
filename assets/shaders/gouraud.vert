#version 330 core

//------------------------------------------------ attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

//---------------------------------------------------- in/out
out vec3 LightingColor;

//-------------------------------------------------- uniforms
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 lightColor;

//------------------------------------------------- matricies
struct    Matrix {

          mat4 local;
          mat4 model;
          mat4 view;
          mat4 projection;
          mat3 normal;
};

struct    Pass {
          
          float opacity;
          float intensity;
          vec3  color;
};

struct    Direction {

          vec3  light;
          vec3  view;
          vec3  reflect;
};

struct    Position {

          vec3 fragment;
          vec3 light;
          vec3 view;
};

uniform   Matrix matrix;

void main() {
    
    mat4  clip_space  = matrix.projection 
                      * matrix.view 
                      * matrix.model 
                      * matrix.local;

    gl_Position       = clip_space * vec4(aPos, 1.0);
    

    Direction direction;
    Position  position;

    position.light = lightPosition;
    position.view  = viewPosition;

    // gouraud shading
    // ---------------------------------------------------------- 
    vec4    position4           = vec4(aPos, 1.0);
            position.fragment   = vec3(matrix.model * position4);

    vec3    Normal              = matrix.normal * aNormal;

    // Ambient
    // ---------------------------------------------------------- 
    Pass    ambient;
            ambient.intensity   = 0.1;
            ambient.color       = ambient.intensity 
                                * lightColor;

    // Diffuse
    // ---------------------------------------------------------- 
    vec3    normal              = normalize(Normal);
            direction.light     = normalize(position.light 
                                          - position.fragment);

    Pass    diffuse;
            diffuse.intensity   = dot(normal, direction.light);
            diffuse.intensity   = max(diffuse.intensity, 0.0);

            diffuse.color       = diffuse.intensity * lightColor;

    // specular
    // ---------------------------------------------------------- 
             direction.view     = normalize  (position.view 
                                            - position.fragment);

             direction.reflect  = reflect    (-direction.light,
                                               normal);

    Pass     specular;
             specular.opacity   = 1.0;
             specular.intensity = dot(direction.view,
                                      direction.reflect);
             specular.intensity = max(specular.intensity, 0.0);
             specular.intensity = pow(specular.intensity, 32);

             specular.color     = specular.opacity 
                                * specular.intensity 
                                * lightColor;
    
    // output
    // ---------------------------------------------------------- 
             LightingColor      = ambient.color 
                                + diffuse.color 
                                + specular.color; 


}
