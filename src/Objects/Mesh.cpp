#include "OBjects/Mesh.h"
#include "Objects/Shader.h"

#include <glad/glad.h>

#include <cstddef>
#include <string>

//--------------------------------------------------------- constructor
Mesh::Mesh( const std::vector<Vertex>       &vertices, 
            const std::vector<unsigned int> &indices,
            const std::vector<MeshTexture>  &textures){

  this->vertices = vertices;
  this->indices  = indices;
  this->textures = textures;

  setupMesh();

}

//---------------------------------------------------------- setup mesh
void Mesh::setupMesh() {
  //-------------------------------------------------- generate buffers
  glGenVertexArrays (1, &VAO);
  glGenBuffers      (1, &VBO);
  glGenBuffers      (1, &EBO); 

  //------------------------------------------------- bind vertex array
  glBindVertexArray (VAO);

  //----------------------------------------------------- vertex buffer
  glBindBuffer( GL_ARRAY_BUFFER, VBO);

  glBufferData( 
    GL_ARRAY_BUFFER, 
    vertices.size() * sizeof(Vertex), 
    vertices.data(),
    GL_STATIC_DRAW  
  );

  //---------------------------------------------------- element buffer
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData( 
    GL_ELEMENT_ARRAY_BUFFER,
    indices.size() * sizeof(unsigned int), 
    indices.data(), 
    GL_STATIC_DRAW  
  );

  //--------------------------------------------------- vertex position
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    0, 
    3, 
    GL_FLOAT, 
    GL_FALSE,
    sizeof(Vertex), 
    reinterpret_cast<void*>(offsetof(Vertex, position)) 
  );

  //----------------------------------------------------- vertex normal
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(  
    1, 
    3, 
    GL_FLOAT, 
    GL_FALSE,  
    sizeof(Vertex), 
    reinterpret_cast<void*>(offsetof(Vertex, normal)) 
  );

  //----------------------------------------------- texture coordinates
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(  
    2, 
    2, 
    GL_FLOAT, 
    GL_FALSE, 
    sizeof(Vertex), 
    reinterpret_cast<void*>(offsetof(Vertex, texCoords)) 
  );

  //----------------------------------------------- unbind vertex array
  glBindVertexArray(0);
}

//---------------------------------------------------------------- draw
void Mesh::draw(Shader &shader){
  
  unsigned int diffuse_num  = 1;
  unsigned int specular_num = 1;
  //----------------------------------------------------- bind textures
  for (unsigned int i = 0; i < textures.size(); i++)
  {
    std::string number;
    std::string name = textures[i].type;

    if           (name == "texture_diffuse")
      number = std::to_string(diffuse_num++);

    else if      (name == "texture_sepcular")
      number = std::to_string(specular_num++);

    shader.setInt("material." + name + number, static_cast<int>(i));

    textures[i].texture->bind(i);
  }

  //--------------------------------------------------------- draw mesh
  glBindVertexArray(VAO);

  glDrawElements(
    GL_TRIANGLES, 
    static_cast<GLsizei>(indices.size()), 
    GL_UNSIGNED_INT, 
    nullptr
  );

  glBindVertexArray(0);

  //------------------------------------------------ reset texture unit
  glActiveTexture(GL_TEXTURE0);
}

