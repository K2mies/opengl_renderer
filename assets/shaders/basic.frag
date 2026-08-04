#version 330 core
out vec4 FragColor;
in  vec3 ourVertexColor;

void main()
{
    FragColor = vec4(ourVertexColor, 1.0);
}
