#version 330 core

//---------------------------------------------------- macros
#define   NUM_OF_POINT_LIGHTS 4

//-------------------------------------------------- uniforms
uniform   vec3        viewPosition;

//---------------------------------------------------- in/out
in        vec3        Normal;
in        vec2        TexCoords;
in        vec4        fragmentPosition;

out       vec4        FragColor;

//--------------------------------------------------- structs
struct    Pass {

          float       strength;
          float       intensity;
          float       shininess;
          vec3        color;
};

struct    Spacial {
          
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

struct    Material {

          sampler2D   diffuse;
          sampler2D   specular;
          sampler2D   emission;
          float       shininess;
};

//---------------------------------------------------- lights
struct    SunLight {

          vec4        direction;
          
          vec3        ambient;
          vec3        diffuse;
          vec3        specular;

};

struct    PointLight {
          
          vec4        position;
    
          float       constant;
          float       linear;
          float       quadratic;
          
          vec3        ambient;
          vec3        diffuse;
          vec3        specular;
};

struct  SpotLight
{
        vec4    position;
        vec4    direction;

        vec3    ambient;
        vec3    diffuse;
        vec3    specular;

        float   constant;
        float   linear;
        float   quadratic;

        float   cutoff;
        float   outer_cutoff;
};

struct    Light {

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

//------------------------------------------- struct uniforms
uniform   Material    material;
uniform   Light       lighting;
uniform   SunLight    sunlight;
uniform   SpotLight   spotlight;
uniform   PointLight  pointlights[NUM_OF_POINT_LIGHTS];


//--------------------------------------- forward declaraions
vec3 calculateSunLight    (SunLight   light, vec3 normal, vec3 view_direction);
vec3 calculateSpotLight   (SpotLight  light, vec3 normal, vec3 frag_position, vec3 view_direction);
vec3 calculatePointLight  (PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction);

void main()
{   
    // Multiple lighting
    // ----------------------------------------------------------
    vec3    normal                    = normalize(Normal);

    Spacial fragment;
            fragment.position         = fragmentPosition;

    Spacial view;
            view.position             = vec4(viewPosition, 1.0);
            view.direction            = normalize(view.position - fragment.position);
    
    // Sun Lighting
    // ----------------------------------------------------------
    vec3    result;
            result                    = calculateSunLight(sunlight, normal, view.direction.xyz) / 2.0f;

    // Point lights
    // ----------------------------------------------------------
    for (int i = 0; i < NUM_OF_POINT_LIGHTS; i++)
    {
      result += calculatePointLight(pointlights[i], normal, fragment.position.xyz, view.direction.xyz) / 2.0f;
    }
  
    // Spot Light
    // ----------------------------------------------------------
      result += calculateSpotLight(spotlight, normal, fragment.position.xyz, view.direction.xyz) * 2.0f; 

    // Output
    // ----------------------------------------------------------
    FragColor = vec4(result, 1.0);
}


//------------------------------------------------- functions
vec3 calculateSunLight(SunLight light, vec3 normal, vec3 view_direction)
{    
    // Sun Light
    // ---------------------------------------------------------- 
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

    // Ambient Shading
    // ---------------------------------------------------------- 
    Pass    ambient;
            ambient.color             = texture(material.diffuse, TexCoords).rgb;
            ambient.color             = sunlight.ambient * ambient.color;

    // Diffuse Shading
    // ---------------------------------------------------------- 
    Pass    diffuse;
            diffuse.intensity         = dot(normal, sunlight.direction.xyz);
            diffuse.intensity         = max(diffuse.intensity, 0.0);
    
            diffuse.color             = texture(material.diffuse, TexCoords).rgb;
            diffuse.color             = sunlight.diffuse 
                                      * diffuse.intensity 
                                      * diffuse.color;
  
    // Specular Shading
    // ---------------------------------------------------------- 
    Pass    specular;
            specular.intensity        = dot(view.direction.xyz, reflection.direction.xyz);
            specular.intensity        = max(specular.intensity, 0.0);
            specular.intensity        = pow(specular.intensity, material.shininess);

            specular.color            = texture(material.specular, TexCoords).rgb;
            specular.color            = sunlight.specular
                                      * specular.intensity
                                      * specular.color;
  
    // Output
    // ---------------------------------------------------------- 
    return (ambient.color + diffuse.color + specular.color); 
}

vec3 calculateSpotLight  (SpotLight light, vec3 normal, vec3 frag_position, vec3 view_direction)
{
    // Point Light
    // ----------------------------------------------------------
    Spacial fragment;
            fragment.position         = vec4      (frag_position, 1.0);
    
    Spacial view;
            view.direction            = vec4      (view_direction, 0.0);

    Spacial spotlight;
            spotlight.position        = light.position;
            spotlight.direction.xyz   = normalize (light.position.xyz - fragment.position.xyz);
            spotlight.direction.w     = 0.0;
  
            spotlight.constant        = light.constant;
            spotlight.linear          = light.linear;
            spotlight.quadratic       = light.quadratic;
  
            spotlight.ambient         = light.ambient;
            spotlight.diffuse         = light.diffuse;
            spotlight.specular        = light.specular;

            spotlight.cutoff          = light.cutoff;
            spotlight.outer_cutoff    = light.outer_cutoff;


    Spacial reflection;
            reflection.direction.xyz  = reflect(-spotlight.direction.xyz, normal);
            reflection.direction.w    = 0.0;

    float   distance                  = length(spotlight.position.xyz - fragment.position.xyz);

    float   attenuation;
            attenuation               = spotlight.constant 
                                      + spotlight.linear 
                                      * distance 
                                      + spotlight.quadratic  
                                      * (distance * distance);
  
            attenuation               = 1.0 / attenuation;

    float   theta;
            theta                     = dot(spotlight.direction.xyz, normalize(-light.direction.xyz));

    float   epsilon                   = spotlight.cutoff
                                      - spotlight.outer_cutoff;

    float   intensity;
            intensity                 = theta - spotlight.outer_cutoff;
            intensity                 = intensity / epsilon;
            intensity                 = clamp(intensity, 0.0, 1.0);
 
    // Ambient Shading
    // ---------------------------------------------------------- 
    Pass    ambient;
            ambient.color             = texture(material.diffuse, TexCoords).rgb;
            ambient.color             = spotlight.ambient * ambient.color;
            ambient.color             *= attenuation;

    // Diffuse Shading
    // ---------------------------------------------------------- 
    Pass    diffuse;
            diffuse.intensity         = dot(normal, spotlight.direction.xyz);
            diffuse.intensity         = max(diffuse.intensity, 0.0);

            diffuse.color             = texture(material.diffuse, TexCoords).rgb;
            diffuse.color             = spotlight.diffuse * diffuse.intensity * diffuse.color;
            diffuse.color             *= attenuation;
            diffuse.color             *= intensity;

    // Specular Shading
    // ---------------------------------------------------------- 
    Pass    specular;
            specular.intensity        = dot(view.direction.xyz, reflection.direction.xyz);
            specular.intensity        = max(specular.intensity, 0.0);
            specular.intensity        = pow(specular.intensity, material.shininess);

            specular.color            = texture(material.specular, TexCoords).rgb;
            specular.color            = spotlight.specular * specular.intensity * specular.color;
            specular.color            *= attenuation;
            specular.color            *= intensity;
    
    // Output
    // ---------------------------------------------------------- 
    return (ambient.color + diffuse.color + specular.color);
}
vec3 calculatePointLight  (PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction)
{
    // Point Light
    // ----------------------------------------------------------
    Spacial fragment;
            fragment.position         = vec4      (frag_position, 1.0);
    
    Spacial view;
            view.direction            = vec4      (view_direction, 0.0);

    Spacial pointlight;
            pointlight.position       = light.position;
            pointlight.direction.xyz  = normalize (light.position.xyz - fragment.position.xyz);
            pointlight.direction.w    = 0.0;

            pointlight.constant       = light.constant;
            pointlight.linear         = light.linear;
            pointlight.quadratic      = light.quadratic;

            pointlight.ambient        = light.ambient;
            pointlight.diffuse        = light.diffuse;
            pointlight.specular       = light.specular;

    Spacial reflection;
            reflection.direction.xyz  = reflect(-pointlight.direction.xyz, normal);
            reflection.direction.w    = 0.0;

    float   distance                  = length(pointlight.position.xyz - fragment.position.xyz);

    float   attenuation;
            attenuation               = pointlight.constant 
                                      + pointlight.linear 
                                      * distance 
                                      + pointlight.quadratic  
                                      * (distance * distance);
  
            attenuation               = 1.0 / attenuation;
 
    // Ambient Shading
    // ---------------------------------------------------------- 
    Pass    ambient;
            ambient.color             = texture(material.diffuse, TexCoords).rgb;
            ambient.color             = pointlight.ambient * ambient.color;
            ambient.color             *= attenuation;

    // Diffuse Shading
    // ---------------------------------------------------------- 
    Pass    diffuse;
            diffuse.intensity         = dot(normal, pointlight.direction.xyz);
            diffuse.intensity         = max(diffuse.intensity, 0.0);

            diffuse.color             = texture(material.diffuse, TexCoords).rgb;
            diffuse.color             = pointlight.diffuse * diffuse.intensity * diffuse.color;
            diffuse.color             *= attenuation;

    // Specular Shading
    // ---------------------------------------------------------- 
    Pass    specular;
            specular.intensity        = dot(view.direction.xyz, reflection.direction.xyz);
            specular.intensity        = max(specular.intensity, 0.0);
            specular.intensity        = pow(specular.intensity, material.shininess);

            specular.color            = texture(material.specular, TexCoords).rgb;
            specular.color            = pointlight.specular * specular.intensity * specular.color;
            specular.color            *= attenuation;
    
    // Output
    // ---------------------------------------------------------- 
    return (ambient.color + diffuse.color + specular.color);
}
