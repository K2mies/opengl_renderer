#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 5) in vec4 aColor;

out vec4 VertexColor;

struct Matrix
{
       mat4   local;
       mat4   model;
       mat4   view;
       mat4   projection;
       mat4   clip;
       mat3   normal;
};

uniform Matrix matrix;

void main()
{
    mat4  clip_space    = matrix.projection 
                        * matrix.view 
                        * matrix.model 
                        * matrix.local;
   
   vec4   worldPosition = matrix.model 
                        * matrix.local 
                        * vec4(aPosition, 1.0);

    vec4  viewPosition  = matrix.view 
                        * worldPosition;
  
    gl_Position         = clip_space * vec4(aPosition, 1.0);
  
    float distanceFromCamera = max(-viewPosition.z, 0.001);

    gl_PointSize        = clamp(8.1 / distanceFromCamera, 0.1, 96.0);

    VertexColor         = aColor;
}

