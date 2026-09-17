#version 330 core

//------------------------------------------------ attributes
layout (location = 0) in vec3 aPosition;
layout (location = 5) in vec4 aColor;

//---------------------------------------------------- output
out   vec4 VertexColor;

//-------------------------------------------------- uniforms

uniform bool   orthographic_projection;
uniform float  orthographic_size;
uniform float  viewport_height;
uniform float  point_world_size;
uniform float  fov;
//-------------------------------------------------- matrices
struct  Matrix
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
  
    if (orthographic_projection)
    {
        gl_PointSize    = point_world_size 
                        * viewport_height 
                        / (2.0 * orthographic_size);
    }
    else
    {
      float distance_from_camera = max(-viewPosition.z,0.001);
      //gl_pointSize      = point_world_size * viewport_height / (2.0 * tan(fov * 0.5) * distance_from_camera);

      gl_PointSize      = fov              * 0.5;
      gl_PointSize      = tan(gl_PointSize);
      gl_PointSize      = 2.0              * gl_PointSize;
      gl_PointSize      = gl_PointSize     * distance_from_camera;
      gl_PointSize      = point_world_size * viewport_height
                        / gl_PointSize;
    }
    //float distanceFromCamera = max(-viewPosition.z, 0.001);

    //gl_PointSize        = clamp(8.1 / distanceFromCamera, 0.1, 96.0);

    VertexColor         = aColor;
}

