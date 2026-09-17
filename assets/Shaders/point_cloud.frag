#version 330 core

in vec4 VertexColor;

out vec4 FragColor;

void main()
{ 
  FragColor = vec4(VertexColor.rgb, 1.0);
}
