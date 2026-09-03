#version 330 core

//-------------------------------------------------- uniforms
uniform   vec3 viewPosition;

//---------------------------------------------------- in/out
in        vec3 Normal;
in        vec2 TexCoords;
in        vec3 fragmentPosition;
out       vec4 FragColor;

//--------------------------------------------------- structs
struct    Pass {

          float strength;
          float intensity;
          float shininess;
          vec3  color;
};

struct    Spacial {
          
          vec3 position;
          vec3 direction;
};

struct    Material {

          sampler2D diffuse;
          sampler2D specular;
          float     shininess;
};

struct    Lighting {

          vec3  position;

          vec3  ambient;
          vec3  diffuse;
          vec3  specular;
};

struct    Light {

          vec3  position;
          vec3  direction;
          
          vec3  ambient;
          vec3  diffuse;
          vec3  specular;
};

uniform   Material material;
uniform   Lighting lighting;

void main()
{ 
    //Direction direction;
    
    Light     light;
              light.position      = lighting.position;
              light.direction     = vec3(0.0);

              light.ambient       = lighting.ambient;
              light.diffuse       = lighting.diffuse;
              light.specular      = lighting.specular;

    Spacial   view;
              view.position       = viewPosition;

    Spacial   fragment;
              fragment.position   = fragmentPosition;

    Spacial   reflection;

    // Ambient
    // ---------------------------------------------------------- 

    Pass  ambient;
          ambient.color             = texture(material.diffuse, TexCoords).rgb;
          ambient.color             = light.ambient * ambient.color;


    
    // Diffuse
    // ---------------------------------------------------------- 
    vec3  normal                  = normalize(Normal);

          light.direction         = light.position
                                  - fragment.position;

          light.direction         = normalize(light.direction);

    Pass  diffuse;
          diffuse.intensity       = dot(normal, light.direction);
          diffuse.intensity       = max(diffuse.intensity, 0.0);

          diffuse.color           = texture(material.diffuse, TexCoords).rgb;
          diffuse.color           = light.diffuse 
                                  * diffuse.intensity 
                                  * diffuse.color;
    
    // specular
    // ---------------------------------------------------------- 
          view.direction          = normalize (  view.position 
                                               - fragment.position);

          reflection.direction    = reflect   ( -light.direction, normal);
    
    Pass  specular;
          specular.intensity      = dot(view.direction, reflection.direction);
          specular.intensity      = max(specular.intensity, 0.0);
          specular.intensity      = pow(specular.intensity, 
                                        material.shininess);

          //specular.color          =  light.specular 
          //                        * (specular.intensity * material.specular);
          specular.color            = texture(material.specular, TexCoords).rgb;
          specular.color            = light.specular 
                                    * specular.intensity 
                                    * specular.color;
          
          //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // output
    // ---------------------------------------------------------- 

    vec3  result;
          result                  =  ambient.color 
                                  +  diffuse.color 
                                  +  specular.color;

          FragColor               =  vec4(result, 1.0);
}
