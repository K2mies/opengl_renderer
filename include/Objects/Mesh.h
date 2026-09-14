#ifndef MESH_H
#define MESH_H

#include<vector>

#include"Objects/Shader.h"
#include"Typedefs/Vertex.h"
#include"Typedefs/MeshTexture.h"

class Mesh
{
public:
  //---------------------------------------------------------------- data
  std::vector<Vertex>       vertices;
  std::vector<unsigned int> indices;
  std::vector<MeshTexture>  textures;

  //--------------------------------------------------------- constructor
  Mesh(const std::vector<Vertex>       &vertices, 
       const std::vector<unsigned int> &indices,
       const std::vector<MeshTexture>  &textures );

  //----------------------------------------------------------- functions
  void draw(Shader& shader);

private:
  //--------------------------------------------------------- render data
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;

  //----------------------------------------------------------- functions
  void setupMesh();
};

#endif
