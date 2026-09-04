#version 330 core

//------------------------------------------------ attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//---------------------------------------------------- in/out
out vec3 Normal;
out vec3 fragmentPosition;
out vec2 TexCoords;
//-------------------------------------------------- uniforms

//------------------------------------------------- matricies
struct    Matrix {

          mat4 local;
          mat4 model;
          mat4 view;
          mat4 projection;
          mat3 normal;
};

uniform   Matrix matrix;

void main() {
    
    mat4  clip_space  = matrix.projection 
                      * matrix.view 
                      * matrix.model 
                      * matrix.local;

    gl_Position       = clip_space * vec4(aPos, 1.0);

    fragmentPosition  = vec3(matrix.model * vec4(aPos, 1.0));

    Normal            = matrix.normal * aNormal;

    TexCoords         = aTexCoords;
}
