#version 330 core

//------------------------------------------------ attributes
layout (location = 0) in vec3 aPos;

//---------------------------------------------------- in/out

//-------------------------------------------------- uniforms

//------------------------------------------------- matricies
struct    Matrix {

          mat4 local;
          mat4 model;
          mat4 view;
          mat4 projection;
};

uniform   Matrix matrix;

void main() {
    
    mat4  clip_space  = matrix.projection 
                      * matrix.view 
                      * matrix.model 
                      * matrix.local;

    gl_Position       = clip_space * vec4(aPos, 1.0);  
}
