#version 330 core

//-------------------------------------------------- uniforms
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

//---------------------------------------------------- in/out
in  vec3 Normal;
in  vec3 fragmentPosition;
out vec4 FragColor;

//--------------------------------------------------- structs
struct    Pass {

          float strength;
          float intensity;
          float shininess;
          vec3  color;
};

struct    Position {
          
          vec3  light;
          vec3  view;
          vec3  fragment;
};

struct    Direction {
         
          vec3 light;
          vec3 view;
          vec3 reflect;

};

struct    Material {

          vec3  ambient;
          vec3  diffuse;
          vec3  specular;
          float shininess;

};

uniform   Material material;

void main()
{ 
    Direction direction;

    Position  position;
              position.light    = lightPosition;
              position.view     = viewPosition;
              position.fragment = fragmentPosition;

    // Ambient
    // ---------------------------------------------------------- 

    //Pass  ambient;
    //      ambient.strength     = 0.1;
    //      ambient.color        = ambient.strength * lightColor;
    Pass    ambient;
            ambient.color      = lightColor * material.ambient;
    
    // Diffuse
    // ---------------------------------------------------------- 
    vec3  normal               = normalize(Normal);

          direction.light      = position.light
                               - position.fragment;
          direction.light      = normalize(direction.light);

    Pass  diffuse;
          diffuse.intensity    = dot(normal, direction.light);
          diffuse.intensity    = max(diffuse.intensity, 0.0);

          //diffuse.color        = diffuse.intensity * lightColor;
          diffuse.color        = lightColor 
                               * (diffuse.intensity * material.diffuse);
    
    // specular
    // ---------------------------------------------------------- 
          direction.view       = normalize (  position.view 
                                            - position.fragment);
          direction.reflect    = reflect   ( -direction.light, normal);
    
    Pass  specular;
          specular.strength    = 0.5;
          specular.shininess   = 32;

          specular.intensity   = dot(direction.view, direction.reflect);
          specular.intensity   = max(specular.intensity, 0.0);
          specular.intensity   = pow(specular.intensity, material.shininess);

          specular.color       = lightColor 
                               * (specular.intensity 
                               * material.specular);

          //specular.color       = specular.strength 
          //                     * specular.intensity
          //                     * lightColor;

    // output
    // ---------------------------------------------------------- 
    vec3  result;
          result              = ambient.color 
                              + diffuse.color 
                              + specular.color;

          //result              = result * objectColor;

          FragColor           = vec4(result, 1.0);

}
