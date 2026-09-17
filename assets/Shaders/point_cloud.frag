#version 330 core

in vec4 VertexColor;

out vec4 FragColor;

void main()
{
  //vec3 displayColor;
  //     displayColor = max(VertexColor.rgb, vec3(0.0, 0.0, 0.0));
  //     displayColor = pow(displayColor,    vec3(1.0 / 2.2));
  
  FragColor = vec4(VertexColor.rgb, 1.0);
}
