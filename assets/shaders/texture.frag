#version 330 core

//---------------------------------------------------- in/out
in      vec3         ourVertexColor;
in      vec2         TexCoord;

out     vec4         FragColor;

//-------------------------------------------------- uniforms
uniform vec4         ourVertexLocation;
uniform sampler2D    ourTexture1;
uniform sampler2D    ourTexture2;

//-------------------------------------------------- weights
const   float        texture_weight    = 0.5;
const   float        vertex_weight     = 0.25;
const   float        location_weight   = 0.25;

//--------------------------------------------------- arrays
const   float        weight[3]         = float[](
                     texture_weight,  // Texture
                     vertex_weight,   // Vertex
                     location_weight  // Location
); 

//---------------------------------------------------- enums
const   int          id_texture        = 0;
const   int          id_vertex         = 1;
const   int          id_location       = 2;

//----------------------------------------------------------

void main() {

  vec4 vertexColor   = vec4 (ourVertexColor, 1.0);

  vec4 texture1      = texture(ourTexture1, TexCoord) / 2.0;
  vec4 texture2      = texture(ourTexture2, TexCoord) / 2.0;
  vec4 mixedTextures = texture1 
                     + texture2;
  
  FragColor          = mixedTextures     *  weight[id_texture]
                     + vertexColor       *  weight[id_vertex]
                     + ourVertexLocation *  weight[id_location];
}
