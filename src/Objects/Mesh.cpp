#include "Objects/Mesh.h"
#include "Objects/Shader.h"

#include <glad/glad.h>

#include <cstddef>
#include <string>

//--------------------------------------------------------- constructor
Mesh::Mesh( const std::vector<Vertex>       &vertices, 
            const std::vector<unsigned int> &indices,
            const std::vector<MeshTexture>  &textures,
                  GLenum                     drawMode)
                : vertices  (vertices),
                  indices   (indices),
                  textures  (textures),
                  drawMode  (drawMode)
{
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
  
  //------------------------------------------------ vertex color

  glEnableVertexAttribArray(5);
  
  glVertexAttribPointer(
      5,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Vertex),
      reinterpret_cast<void*>(
          offsetof(Vertex, color)
      )
  );

  //----------------------------------------------- unbind vertex array
  glBindVertexArray(0);
}

//---------------------------------------------------------------- draw
void Mesh::draw(Shader &shader){
  
  unsigned int diffuse_num   = 1;
  unsigned int specular_num  = 1;
  unsigned int emission_num  = 1;

  bool         hasDiffuseMap = false;
  //----------------------------------------------------- bind textures
  for (unsigned int i = 0; i < textures.size(); i++)
  {
    std::string number;
    std::string name;

    if (textures[i].type == diffuse){
      number = std::to_string(diffuse_num++);
      name   = "texture_diffuse";

      hasDiffuseMap = true;
    }

    if (textures[i].type == specular){
      number = std::to_string(specular_num++);
      name   = "texture_specular";
    }

    if (textures[i].type == emission){
      number = std::to_string(emission_num++);
      name   = "texture_emission";
    }

    shader.setInt("material." + name + number, static_cast<int>(i));

    textures[i].texture->bind(i);
  }

  shader.setBool("material.hasDiffuseMap", hasDiffuseMap);

  //--------------------------------------------------------- draw mesh
  glBindVertexArray(VAO);

  if (!indices.empty())
  {
    glDrawElements(
      drawMode,
      static_cast<GLsizei>(indices.size()),
      GL_UNSIGNED_INT,
      nullptr
    );
  }
  else
  {
    glDrawArrays(
      drawMode,
      0,
      static_cast<GLsizei>(vertices.size())
    );
  }
  //glDrawElements(
  //  GL_TRIANGLES, 
  //  static_cast<GLsizei>(indices.size()), 
  //  GL_UNSIGNED_INT, 
  //  nullptr
  //);

  glBindVertexArray(0);

  //------------------------------------------------ reset texture unit
  glActiveTexture(GL_TEXTURE0);
}

