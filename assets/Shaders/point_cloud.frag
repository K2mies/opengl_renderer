#version 330 core

//----------------------------------------------------- input
in vec4 VertexColor;
in vec3 Normal;
in vec4 FragmentPosition;

//---------------------------------------------------- output
out vec4 FragColor;

//-------------------------------------------------- uniforms
uniform vec3 viewPosition;
uniform vec3 modelCenter;

uniform int  lighting_type;

//----------------------------------------------------- enums
const int phong   = 0;
const int gouraud = 1;

//--------------------------------------------------- structs
struct    Pass 
{

          float       strength;
          float       intensity;
          float       shininess;
          vec3        color;
};

struct    Spacial 
{
          
          vec4        position;
          vec4        direction;

          vec3        ambient;
          vec3        diffuse;
          vec3        specular;

          float       constant;
          float       linear;
          float       quadratic;

          float       cutoff;
          float       outer_cutoff;
};

struct    Material 
{
          vec3        diffuse;
          vec3        specular;
          float       shininess;

};

//---------------------------------------------------- lights
struct    SunLight 
{

          vec4        direction;
          
          vec3        ambient;
          vec3        diffuse;
          vec3        specular;

};

//-------------------------------------------- struct uniforms
uniform SunLight sunlight;
uniform Material material;

//--------------------------------------- forward declarations
vec3 calculateSunLight(SunLight   light, vec3 normal, vec3 view_direction, vec3 vertex_color);

//-------------------------------------------------------------------------------------------- main
void main()
{ 
  if (lighting_type == phong)
  {
    vec3 normal         = normalize        (FragmentPosition.xyz - modelCenter);
    vec3 view_direction = normalize        (viewPosition - FragmentPosition.xyz);
    vec3 lighting       = calculateSunLight(sunlight, normal, view_direction, VertexColor.rgb);

    FragColor = vec4(lighting, 1.0);
  }

  if (lighting_type == gouraud)
  {
    FragColor = vec4(VertexColor.rgb, 1.0);
  }
}

//--------------------------------------------------------------------------------------- functions
vec3 calculateSunLight(SunLight light, vec3 normal, vec3 view_direction, vec3 vertex_color)
{    
    // Sun Light
    // ------------------------------------------------------------------------------------------------ 
    Spacial sunlight;
            sunlight.direction.xyz    = normalize (-light.direction.xyz);
            sunlight.direction.w      = 0.0;

            sunlight.ambient          = light.ambient;
            sunlight.diffuse          = light.diffuse;
            sunlight.specular         = light.specular;

    Spacial reflection;
            reflection.direction.xyz  = reflect (-sunlight.direction.xyz, normal);
            reflection.direction.w    = 0.0;

    Spacial view;
            view.direction            = vec4      (view_direction, 0.0);

    // Base Color
    // ------------------------------------------------------------------------------------------------ 
    Pass    base;
            //base.color                = vertex_color * material.diffuse;
            base.color                = vertex_color;
    
    // Ambient Shading
    // ------------------------------------------------------------------------------------------------ 
    Pass    ambient;
            ambient.color             = sunlight.ambient * base.color;

    // Diffuse Shading
    // ------------------------------------------------------------------------------------------------ 
    Pass    diffuse;
            diffuse.intensity         = dot(normal, sunlight.direction.xyz);
            diffuse.intensity         = max(diffuse.intensity, 0.0);
    
            diffuse.color             = sunlight.diffuse 
                                      * diffuse.intensity 
                                      * base.color;
  
    // Specular Shading
    // ------------------------------------------------------------------------------------------------ 
    Pass    specular;
            specular.intensity        = dot(view.direction.xyz, reflection.direction.xyz);
            specular.intensity        = max(specular.intensity, 0.0);
            specular.intensity        = pow(specular.intensity, material.shininess);

            specular.color            = sunlight.specular
                                      * specular.intensity
                                      * material.specular;
  
    // Output
    // ------------------------------------------------------------------------------------------------ 
    return (ambient.color + diffuse.color + specular.color); 
}
