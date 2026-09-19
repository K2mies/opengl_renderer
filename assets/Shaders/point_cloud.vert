#version 330 core

//------------------------------------------------ attributes
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 5) in vec4 aColor;

//---------------------------------------------------- output
out    vec4   VertexColor;
out    vec3   Normal;
out    vec4   FragmentPosition;

//-------------------------------------------------- uniform
uniform int   lighting_type;
uniform vec3  viewPosition;
uniform vec3  modelCenter;

//----------------------------------------------------- enums
//projection type
const   int   orthographic = 0;
const   int   perspective  = 1;

//lighting type
const   int   phong        = 0;
const   int   gouraud      = 1;

//-------------------------------------------------- matrices

struct  Projection
{
        int    type;
        float  size;
        float  height;
        float  point_size;
        float  fov;
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

struct  Material
{
        vec3  diffuse;
        vec3  specular;
        float shininess;
};

struct  SunLight
{
        vec4  direction;
        
        vec3  ambient;
        vec3  diffuse;
        vec3  specular;
};

//-------------------------------------------- struct uniforms
uniform Matrix   matrix;
uniform Material material;
uniform SunLight sunlight;

//--------------------------------------- forward declarations
vec3 calculateSunLight(SunLight light, vec3 normal, vec3 view_direction, vec3 vertex_color);

void main()
{
    //-------------------------------------------------- position calculations
    mat4  clip_space              = matrix.projection 
                                  * matrix.view 
                                  * matrix.model 
                                  * matrix.local;
   
    vec4  worldPosition           = matrix.model 
                                  * matrix.local 
                                  * vec4(aPosition, 1.0);
 
    vec4  cameraSpacePosition     = matrix.view 
                                  * worldPosition;
  
    gl_Position                   = clip_space * vec4(aPosition, 1.0);
 
    //------------------------------------------------ point size calculations
    if (projection.type == orthographic)
    {
      float visible_world_height  = 2.0 * projection.size;
      float pixels_per_world_unit = projection.height / visible_world_height;

      gl_PointSize                = projection.point_size * pixels_per_world_unit;
    }

    if (projection.type == perspective)
    {
      float distance_from_camera  = max(-cameraSpacePosition.z, 0.001);

      float half_fov              = projection.fov * 0.5;
      float half_view_height      = tan(half_fov) * distance_from_camera;
      float visible_world_height  = 2.0 * half_view_height;
      float pixels_per_world_unit = projection.height / visible_world_height;

      gl_PointSize                = projection.point_size * pixels_per_world_unit;
    }

    FragmentPosition              = worldPosition;
    
    Normal                        = normalize(worldPosition.xyz - modelCenter);

    //-------------------------------------------------------- lighting models
    
    // PHONG
    if (lighting_type == phong)
    {
        VertexColor         = aColor;
    }
   
    // GOURAUD
    if (lighting_type == gouraud)
    {
        vec3 view_direction = normalize(viewPosition - worldPosition.xyz);

        vec3 lighting       = calculateSunLight(sunlight,
                                                Normal, 
                                                view_direction, 
                                                aColor.rgb);

        VertexColor         = vec4(lighting, aColor.a);
        
    }
}

//------------------------------------------------------------------ functions
vec3 calculateSunLight(SunLight light, vec3 normal, vec3 view_direction, vec3 vertex_color)
{
  //--------------------------------------------------------------- directions
  vec3  light_direction = normalize(-light.direction.xyz);
  vec3  reflection_direction = reflect(-light_direction.xyz, normal);

  //-------------------------------------------------------------- base colour
  vec3  base_color = vertex_color;

  //------------------------------------------------------------------ ambient
  vec3  ambient = light.ambient * base_color;

  //------------------------------------------------------------------ diffuse
  float diffuse_intensity;
        diffuse_intensity = dot(normal, light_direction);
        diffuse_intensity = max(diffuse_intensity, 0.0);
        
  vec3  diffuse = light.diffuse * diffuse_intensity * base_color;

  //----------------------------------------------------------------- specular
  float specular_intensity;
        specular_intensity = dot(view_direction, reflection_direction);
        specular_intensity = max(specular_intensity, 0.0);
        specular_intensity = pow(specular_intensity, material.shininess);

  vec3  specular = light.specular * specular_intensity * material.specular;

  //------------------------------------------------------------------- result
  return ambient + diffuse + specular;
}

