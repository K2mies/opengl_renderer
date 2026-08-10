#version 330 core

//---------------------------------------------------- in/out
in      vec3         ourVertexColor;
in      vec2         TexCoord;

out     vec4         FragColor;

//-------------------------------------------------- uniforms
uniform vec4         ourVertexLocation;

uniform sampler2D    ourTexture1;
uniform sampler2D    ourTexture2;

//-------------------------------------------------- structs
struct  Weight {

        float        texture;
        float        vertex;
        float        location;

        float        blend;

};

uniform Weight       weight;

//----------------------------------------------------------

void main() {

  vec4 vertexColor   = vec4   (ourVertexColor, 1.0);

  vec4 texture1      = texture(ourTexture1,  TexCoord) / 2.0;
  vec4 texture2      = texture(ourTexture2,  TexCoord) / 2.0;
  vec4 mixed         = mix    (texture1,     texture2,

                                             weight.blend);
  
  FragColor          = mixed              *  weight.texture
                     + vertexColor        *  weight.vertex
                     + ourVertexLocation  *  weight.location;
}
