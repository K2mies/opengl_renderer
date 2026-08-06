#version 330 core

//------------------------------------------------ attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

//---------------------------------------------------- in/out
out     vec3 ourVertexColor;
out     vec2 TexCoord;

//-------------------------------------------------- uniforms
uniform vec4 ourVertexLocation;

void main()
{
    gl_Position    = vec4(aPos, 1.0) + ourVertexLocation;
    ourVertexColor = aColor;
    TexCoord       = aTexCoord;
}
