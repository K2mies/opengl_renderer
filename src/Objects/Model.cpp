#include "Objects/Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>


Model::Model(const std::string &path){
  loadModel(path);
}

void Model::draw(Shader &shader){
  for (Mesh &mesh : meshes)
    mesh.draw(shader);
}

void Model::loadModel(const std::string &path){

  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile( 
    path, 
    aiProcess_Triangulate 
  | aiProcess_FlipUVs 
  | aiProcess_GenSmoothNormals
  );

  if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0 || scene->mRootNode == nullptr){
    std::cerr << "ERROR::ASSIMP:"
              << importer.GetErrorString()
              << std::endl;
    return ;
              
  }

  const std::size_t separator = path.find_last_of("/\\");

  directory = separator == std::string::npos ? "." : path.substr(0, separator);
  
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene){

  for (unsigned int i = 0; i < node->mNumMeshes; ++i)
  {
    const unsigned int meshIndex = node->mMeshes[i];
    aiMesh *assimpMesh = scene->mMeshes[meshIndex];

    meshes.push_back(processMesh(assimpMesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++){
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene){
  std::vector<Vertex>           vertices;
  std::vector<unsigned int>     indices;
  std::vector<MeshTexture>      textures;

  //---------------------------------------------------- vertices

  vertices.reserve(mesh->mNumVertices);

  for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
  {
    Vertex vertex{};

    //-------------------------------------------------- position
    vertex.position = vec3( mesh->mVertices[i].x, 
                            mesh->mVertices[i].y, 
                            mesh->mVertices[i].z);

    //---------------------------------------------------- normal
    if (mesh->HasNormals()){
      vertex.normal = vec3( mesh->mNormals[i].x, 
                            mesh->mNormals[i].y, 
                            mesh->mNormals[i].z);
    }
    else{
      vertex.normal = vec3(0.0f);
    }

    //--------------------------------------- texture coordinates
    
    if (mesh->HasTextureCoords(0)){
      vertex.texCoords = vec2(mesh->mTextureCoords[0][i].x, 
                              mesh->mTextureCoords[0][i].y);
    }
    else{
      vertex.texCoords = vec2(0.0f);
    }

    vertices.push_back(vertex);
  }

  //------------------------------------------------------ indices

  for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
  {
    const aiFace &face = mesh->mFaces[i];

    for (unsigned int j = 0; j < face.mNumIndices; ++j)
    {
      indices.push_back(face.mIndices[j]);
    }
  }

  //----------------------------------------------------- textures

    /*
     * Material texture loading will go here after we add
     * loadMaterialTextures().
     */
  //-------------------------------------------------- create mesh
  
  return Mesh(vertices, indices, textures);

}
