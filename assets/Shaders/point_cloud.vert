#version 330 core

//------------------------------------------------ attributes
layout (location = 0) in vec3 aPosition;
layout (location = 5) in vec4 aColor;

//---------------------------------------------------- output
out   vec4 VertexColor;

//-------------------------------------------------- uniforms
//uniform bool   orthographic_projection;
//uniform float  orthographic_size;
//uniform float  viewport_height;
//uniform float  point_world_size;
//uniform float  fov;

//----------------------------------------------------- enums
const int orthographic = 0;
const int perspective  = 1;
const int polar        = 2;

//-------------------------------------------------- matrices

struct  Projection
{
        int   type;
        float size;
        float height;
        float point_size;
        float fov;
};

uniform Projection projection;

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
  
    if (projection.type == orthographic)
    {
      float visible_world_height  = 2.0 * projection.size;
      float pixels_per_world_unit = projection.height / visible_world_height;

      gl_PointSize                = projection.point_size * pixels_per_world_unit;

      //float point_size;
      //      point_size = 2.0 * projection.size;
      //      point_size = projection.height / point_size;
      //      point_size = projection.point_size * point_size;

      //gl_PointSize = point_size;

    }
    if (projection.type == perspective)
    {
      float distance_from_camera  = max(-viewPosition.z, 0.001);

      float half_fov              = projection.fov * 0.5;
      float half_view_height      = tan(half_fov) * distance_from_camera;
      float visible_world_height  = 2.0 * half_view_height;
      float pixels_per_world_unit = projection.height / visible_world_height;

      gl_PointSize                = projection.point_size * pixels_per_world_unit;

      //float distance_from_camera = max(-viewPosition.z, 0.001);

      //float point_size;
      //      point_size = projection.fov * 0.5;
      //      point_size = tan(point_size) * distance_from_camera;
      //      point_size = 2.0 * point_size;
      //      point_size = projection.height / point_size;
      //      point_size = projection.point_size * point_size;

      //gl_PointSize     = point_size;
    }

    VertexColor         = aColor;
}

