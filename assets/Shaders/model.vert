#version 330 core

//------------------------------------------------ attributes

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

//---------------------------------------------------- output

out vec2 TexCoords;

//-------------------------------------------------- matrices
   
struct Matrix
{
       mat4 local;
       mat4 model;
       mat4 view;
       mat4 projection;
       mat4 clip;
       mat3 normal;
};

uniform Matrix matrix;  

//------------------------------------------------------ main

void main()
{
  TexCoords = aTexCoords;
  
  mat4  clip_space  = matrix.projection 
                    * matrix.view 
                    * matrix.model 
                    * matrix.local;
  
  gl_Position       = clip_space * vec4(aPosition, 1.0);


}

