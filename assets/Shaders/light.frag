#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

struct    PointLight {
          
          vec4        position;
    
          float       constant;
          float       linear;
          float       quadratic;
          
          vec3        ambient;
          vec3        diffuse;
          vec3        specular;
};

uniform   PointLight  light;

void main()
{
    FragColor   = vec4(light.diffuse, 1.0);
}
