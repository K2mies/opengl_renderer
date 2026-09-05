#version 330 core

//-------------------------------------------------- uniforms
uniform   vec3        viewPosition;

//---------------------------------------------------- in/out
in        vec3        Normal;
in        vec2        TexCoords;
in        vec3        fragmentPosition;

out       vec4        FragColor;

//--------------------------------------------------- structs
struct    Pass {

          float       strength;
          float       intensity;
          float       shininess;
          vec3        color;
};

struct    Spacial {
          
          vec3        position;
          vec3        direction;

          vec3        ambient;
          vec3        diffuse;
          vec3        specular;
};

struct    Material {

          sampler2D   diffuse;
          sampler2D   specular;
          sampler2D   emission;
          float       shininess;
};

struct    Light {

          vec3        position;
          vec3        direction;

          vec3        ambient;
          vec3        diffuse;
          vec3        specular;
};

//------------------------------------------- struct uniforms
uniform   Material    material;
uniform   Light       lighting;

void main()
{   
    // Phong lighting
    // ---------------------------------------------------------- 
    vec3      normal                = normalize(Normal);

    Spacial   fragment;
              fragment.position     = fragmentPosition;

    Spacial   light;
              light.position        = lighting.position;

              //light.direction       = light.position - fragment.position;
              //light.direction       = normalize(light.direction);
              light.direction       = normalize(-lighting.direction);

              light.ambient         = lighting.ambient;
              light.diffuse         = lighting.diffuse;
              light.specular        = lighting.specular;
    
    Spacial   view;
              view.position         = viewPosition;

              view.direction        = view.position - fragment.position;
              view.direction        = normalize(view.direction);

    Spacial   reflection;
              reflection.direction  = reflect(-light.direction, normal);

    // Ambient
    // ---------------------------------------------------------- 

    Pass      ambient;
              ambient.color         = texture(material.diffuse, TexCoords).rgb;
              ambient.color         = light.ambient * ambient.color;

    // Diffuse
    // ---------------------------------------------------------- 

    Pass      diffuse;
              diffuse.intensity     = dot(normal, light.direction);
              diffuse.intensity     = max(diffuse.intensity, 0.0);

              diffuse.color         = texture(material.diffuse, TexCoords).rgb;
              diffuse.color         = light.diffuse 
                                    * diffuse.intensity 
                                    * diffuse.color;
    
    // specular
    // ---------------------------------------------------------- 

    Pass      specular;
              specular.intensity    = dot(view.direction, reflection.direction);
              specular.intensity    = max(specular.intensity, 0.0);
              specular.intensity    = pow(specular.intensity, material.shininess);

              specular.color        = texture(material.specular, TexCoords).rgb;
              //specular.color        = vec3(1.0, 1.0, 1.0) - specular.color;  //negate the specular map
              specular.color        = light.specular 
                                    * specular.intensity 
                                    * specular.color;

    // emission
    // ---------------------------------------------------------- 
    
    Pass       emission;
               emission.color       = texture(material.emission, TexCoords).rgb;

    // output
    // ---------------------------------------------------------- 

    vec3       result;
               result               =  ambient.color 
                                    +  diffuse.color
                                    +  specular.color;
                                    //*  emission.color;

               FragColor            =  vec4(result, 1.0);
}
