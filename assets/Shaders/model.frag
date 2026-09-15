#version 330 core

//---------------------------------------------------- input

in vec2 TexCoords;

//--------------------------------------------------- output

out vec4 FragColor;

//------------------------------------------------- material

struct Material
{
    sampler2D texture_diffuse1;
    
    bool      hasDiffuseMap;
    
    vec3      color;
};

uniform Material material;

//------------------------------------------------------- main

void main()
{
  if (material.hasDiffuseMap)
  {
    FragColor = texture(material.texture_diffuse1,TexCoords);
  }
  else
  {
    FragColor = vec4(material.color, 1.0);
  }
}
