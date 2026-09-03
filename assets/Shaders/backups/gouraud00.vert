#version 330 core

//------------------------------------------------ attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

//---------------------------------------------------- in/out
out vec3 LightingColor;

//-------------------------------------------------- uniforms
uniform vec3 viewPosition;

//------------------------------------------------- matricies
struct    Matrix {

          mat4 local;
          mat4 model;
          mat4 view;
          mat4 projection;
          mat3 normal;
};

struct    Pass {
          
          float intensity;
          vec3  color;
};

struct    Spacial {
        
          vec3 position;
          vec3 direction;

};

struct    Material {

          vec3  ambient;
          vec3  diffuse;
          vec3  specular;
          float shininess;

};

struct    Lighting  {

          vec3  position;
          
          vec3  ambient;
          vec3  diffuse;
          vec3  specular;
};

struct    Light {

          vec3 position;
          vec3 direction;
    
          vec3  ambient;
          vec3  diffuse;
          vec3  specular;
};

uniform   Matrix   matrix;
uniform   Material material;
uniform   Lighting lighting;

void main() {
    
    mat4  clip_space  = matrix.projection 
                      * matrix.view 
                      * matrix.model 
                      * matrix.local;

    gl_Position       = clip_space * vec4(aPos, 1.0);
    
    Light     light;
              light.position    = lighting.position;
              light.direction   = vec3(0.0);

              light.ambient     = lighting.ambient;
              light.diffuse     = lighting.diffuse;
              light.specular    = lighting.specular;

    Spacial   view;
              view.position     = viewPosition;

    Spacial   fragment;
    Spacial   reflection;

    // gouraud shading
    // ---------------------------------------------------------- 
    vec4    position4           = vec4(aPos, 1.0);
            fragment.position   = vec3(matrix.model * position4);

    vec3    Normal              = matrix.normal * aNormal;

    // Ambient
    // ---------------------------------------------------------- 
    Pass    ambient;
            ambient.color       = light.ambient * material.ambient; 

    // Diffuse
    // ---------------------------------------------------------- 
    vec3    normal              = normalize(Normal);
            light.direction     = normalize(light.position 
                                          - fragment.position);

    Pass    diffuse;
            diffuse.intensity   = dot(normal, light.direction);
            diffuse.intensity   = max(diffuse.intensity, 0.0);

            diffuse.color       = light.diffuse 
                                * (diffuse.intensity * material.diffuse);

    // specular
    // ---------------------------------------------------------- 
             view.direction        = normalize  ( view.position 
                                                - fragment.position);

             reflection.direction  = reflect    (-light.direction,
                                                  normal);

    Pass     specular;
             specular.intensity = dot(view.direction,
                                      reflection.direction);
             specular.intensity = max(specular.intensity, 0.0);
             specular.intensity = pow(specular.intensity, material.shininess);

             specular.color     = light.specular
                                * (specular.intensity * material.specular);
    
    // output
    // ---------------------------------------------------------- 
             LightingColor      = ambient.color 
                                + diffuse.color 
                                + specular.color; 


}
