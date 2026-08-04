#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec4 ourVertexLocation;
out     vec3 ourVertexColor;

void main() {
  gl_Position = vec4(aPos.x - 0.5, aPos.y, aPos.z, 1.0) + ourVertexLocation;
  ourVertexColor = aColor;
}
