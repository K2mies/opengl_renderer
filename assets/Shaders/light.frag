#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

struct    Light {

          vec4        position;
          vec4        direction;

          vec3        ambient;
          vec3        diffuse;
          vec3        specular;
          
          float       constant;
          float       linear;
          float       quadratic;
};

uniform Light  lighting;

void main()
{
    FragColor   = vec4(lighting.diffuse, 1.0);
}
