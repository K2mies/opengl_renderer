#version 330 core

//------------------------------------------------ attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

//---------------------------------------------------- in/out
out     vec3  ourVertexColor;
out     vec2  TexCoord;

//-------------------------------------------------- uniforms
uniform   vec4  ourVertexLocation;
uniform   mat4  transform;

//------------------------------------------------- matricies
struct    Matrix {

          mat4 local;
          mat4 model;
          mat4 view;
          mat4 projection;
          mat4 clip;
};

uniform Matrix matrix;

void main()
{
    gl_Position    = matrix.projection 
                   * matrix.view 
                   * matrix.model
                   * matrix.local
                   * vec4(aPos, 1.0);
  
    ourVertexColor = aColor;
    TexCoord       = aTexCoord;
}
