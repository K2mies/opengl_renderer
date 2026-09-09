#version 330 core

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

void main()
{   
    // Spotlight lighting
    // ---------------------------------------------------------- 
    vec3      normal                   = normalize(Normal);

    Spacial   fragment;
              fragment.position        = fragmentPosition;

    Spacial   light;
              light.position           = lighting.position;
 
              light.direction.xyz      = light.position.xyz - fragment.position.xyz;
              light.direction.xyz      = normalize(light.direction.xyz);
              light.direction          = vec4(light.direction.xyz, 0.0);
              
              // for direction light switch to this
              //light.direction.xyz      = normalize(-lighting.direction.xyz);
              //light.direction          = vec4(light.direction.xyz, 0.0);

              light.ambient            = lighting.ambient;
              light.diffuse            = lighting.diffuse;
              light.specular           = lighting.specular;

              light.constant           = lighting.constant;
              light.linear             = lighting.linear;
              light.quadratic          = lighting.quadratic;

              light.cutoff             = lighting.cutoff;
              light.outer_cutoff       = lighting.outer_cutoff;
    
    Spacial   view;
              view.position            = vec4(viewPosition, 1.0);

              view.direction.xyz       = view.position.xyz - fragment.position.xyz;
              view.direction.xyz       = normalize(view.direction.xyz);
              view.direction           = vec4(view.direction.xyz, 0.0);

    Spacial   reflection;
              reflection.direction.xyz = reflect(-light.direction.xyz, normal);
              reflection.direction     = vec4(reflection.direction.xyz, 0.0);

    float     distance                 = length(light.position - fragment.position);

    float     attenuation;
              attenuation              = light.constant 
                                       + light.linear 
                                       * distance 
                                       + light.quadratic  
                                       * (distance * distance);
              attenuation              = 1.0 / attenuation;

    float     theta;
              theta                    = dot( light.direction.xyz, normalize(-lighting.direction.xyz) );

    float     epsilon;
              epsilon                  = light.cutoff - light.outer_cutoff;

    float     intensity;
              intensity                = (theta - light.outer_cutoff) / epsilon;
              intensity                = clamp(intensity, 0.0, 1.0);
    

    // Ambient
    // ---------------------------------------------------------- 

    Pass      ambient;
              ambient.color            = texture(material.diffuse, TexCoords).rgb;
              ambient.color            = light.ambient * ambient.color;

    // Diffuse
    // ---------------------------------------------------------- 

    Pass      diffuse;
              diffuse.intensity        = dot(normal, light.direction.xyz);
              diffuse.intensity        = max(diffuse.intensity, 0.0);

              diffuse.color            = texture(material.diffuse, TexCoords).rgb;
              diffuse.color            = light.diffuse 
                                       * diffuse.intensity 
                                       * diffuse.color;
    
    // specular
    // ---------------------------------------------------------- 

    Pass      specular;
              specular.intensity       = dot(view.direction.xyz, reflection.direction.xyz);
              specular.intensity       = max(specular.intensity, 0.0);
              specular.intensity       = pow(specular.intensity, material.shininess);

              specular.color           = texture(material.specular, TexCoords).rgb;
              //specular.color           = vec3(1.0, 1.0, 1.0) - specular.color;  //negate the specular map
              specular.color           = light.specular 
                                       * specular.intensity 
                                       * specular.color;

    // emission
    // ---------------------------------------------------------- 
    
    Pass       emission;
               emission.color         = texture(material.emission, TexCoords).rgb;

    // output
    // ---------------------------------------------------------- 
    
    vec3       result;

    //if (theta > lighting.cutoff)
    //{
    //           ambient.color          *= attenuation;
    //           diffuse.color          *= attenuation;
    //           specular.color         *= attenuation;

    //           result                 = ambient.color
    //                                  + diffuse.color
    //                                  + specular.color;
    //}
    //else
    //{
    //           result                 = ambient.color;
    //}
               diffuse.color          =  diffuse.color  * intensity * attenuation;
               specular.color         =  specular.color * intensity * attenuation;

               result                 = ambient.color
                                      + diffuse.color
                                      + specular.color;

               FragColor              =  vec4(result, 1.0);
}
