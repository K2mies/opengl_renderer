#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

struct  Light {

        vec3  position;
        
        vec3  ambient;
        vec3  diffuse;
        vec3  specular;

};

uniform Light  light;

void main()
{
    FragColor   = vec4(light.diffuse, 1.0);
}
