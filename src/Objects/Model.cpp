#include "Objects/Model.h"
#include "Typedefs/MeshTexture.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>
#include <string>
#include <vector>


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
    aiProcess_SortByPType       |
    aiProcess_Triangulate       | 
    aiProcess_FlipUVs           |
    aiProcess_GenSmoothNormals  |
    aiProcess_CalcTangentSpace
  );

  if (scene == nullptr)
  {
      std::cerr
          << "ERROR::ASSIMP: scene is null\n"
          << importer.GetErrorString()
          << '\n';
  
      return;
  }
  
  if (scene->mRootNode == nullptr)
  {
      std::cerr
          << "ERROR::ASSIMP: scene has no root node\n"
          << importer.GetErrorString()
          << '\n';
  
      return;
  }
  

  if ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0)
  {
      std::cerr
          << "WARNING::ASSIMP: scene is marked incomplete\n"
          << "Scene flags: "
          << scene->mFlags
          << '\n';
  }

  std::cout
      << "Scene meshes: "
      << scene->mNumMeshes
      << '\n'
      << "Scene materials: "
      << scene->mNumMaterials
      << '\n'
      << "Root meshes: "
      << scene->mRootNode->mNumMeshes
      << '\n'
      << "Root children: "
      << scene->mRootNode->mNumChildren
      << '\n';

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
  
  //----------------------------------------------------------------------------------- diagnostics
  std::cout
    << "Has normals: "
    << (mesh->HasNormals() ? "true" : "false")
    << '\n';

  std::cout
      << "Has vertex colors: "
      << std::boolalpha
      << mesh->HasVertexColors(0)
      << '\n';

  std::cout
      << "Mesh vertices: "
      << mesh->mNumVertices
      << ", faces: "
      << mesh->mNumFaces
      << ", primitive types: "
      << mesh->mPrimitiveTypes
      << '\n';


  std::vector<Vertex>           vertices;
  std::vector<unsigned int>     indices;
  std::vector<MeshTexture>      textures;

  //------------------------------------------------------------------------------------- vertices

  vertices.reserve(mesh->mNumVertices);

  for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
  {
    Vertex vertex{};

    //----------------------------------------------------------------------------------- position
    vertex.position.x   = mesh->mVertices[i].x;
    vertex.position.y   = mesh->mVertices[i].y;
    vertex.position.z   = mesh->mVertices[i].z;
    
    //------------------------------------------------------------------------------------- normal
    if (mesh->HasNormals())
    {
      vertex.normal.x   = mesh->mNormals[i].x;
      vertex.normal.y   = mesh->mNormals[i].y;
      vertex.normal.z   = mesh->mNormals[i].z;
    }
    else
    {
      vertex.normal.x   = 0.0f;
      vertex.normal.y   = 0.0f;
      vertex.normal.z   = 0.0f;
    }

    //------------------------------------------------------------------------ texture coordinates
    
    if (mesh->HasTextureCoords(0))
    {
      vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
      vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
    }
    else
    {
      vertex.texCoords.x = 0.0f;
      vertex.texCoords.y = 0.0f;
    }
    //------------------------------------------------------------------------ tangent and bitangent
    if (mesh->HasTangentsAndBitangents())
    {
      vertex.tangent.x   = mesh->mTangents[i].x;
      vertex.tangent.y   = mesh->mTangents[i].y;
      vertex.tangent.z   = mesh->mTangents[i].z;
      
      vertex.bitangent.x = mesh->mBitangents[i].x;
      vertex.bitangent.y = mesh->mBitangents[i].y;
      vertex.bitangent.z = mesh->mBitangents[i].z;
    }
    else
    {
      vertex.tangent.x   = 0.0f;
      vertex.tangent.y   = 0.0f;
      vertex.tangent.z   = 0.0f;

      vertex.bitangent.x = 0.0f;
      vertex.bitangent.y = 0.0f;
      vertex.bitangent.z = 0.0f;
    }

    //---------------------------------------------------------------------------- vertex color 
    if (mesh->HasVertexColors(0))
    {
        const aiColor4D& color =
            mesh->mColors[0][i];
    
        vertex.color.x = color.r;
        vertex.color.y = color.g;
        vertex.color.z = color.b;
        vertex.color.w = color.a;
    }
    else
    {
        // Use white when the model has no vertex colors.
        vertex.color.x = 1.0f;
        vertex.color.y = 1.0f;
        vertex.color.z = 1.0f;
        vertex.color.w = 1.0f;
    }

    //--------------------------------------------------------------------------------- store vertex
    vertices.push_back(vertex);
  }

  //--------------------------------------------------------------------------------------- indices
  
  indices.reserve(mesh->mNumFaces * 3);

  for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
  {
    const aiFace &face = mesh->mFaces[i];

    for (unsigned int j = 0; j < face.mNumIndices; ++j)
    {
      indices.push_back(face.mIndices[j]);
    }
  }

  //-------------------------------------------------------------------------------------- textures

    /*
     * Material texture loading will go here after we add
     * loadMaterialTextures().
     */

  //-------------------------------------------------------------------------------------- material
  if (mesh->mMaterialIndex < scene->mNumMaterials)
  {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    //-------------------------------------------------------------------------------- diffuse maps
    std::vector<MeshTexture> diffuseMaps =
                     loadMaterialTextures( material, 
                                           aiTextureType_DIFFUSE, 
                                           diffuse);

    textures.insert(textures.end(), 
                    diffuseMaps.begin(), 
                    diffuseMaps.end());

    //------------------------------------------------------------------------------- specular maps
    std::vector<MeshTexture> specularMaps =
                    loadMaterialTextures( material,
                                          aiTextureType_SPECULAR,
                                          specular);

    textures.insert(textures.end(),
                    specularMaps.begin(),
                    specularMaps.end());

    //------------------------------------------------------------------------------- emission maps
    std::vector<MeshTexture> emissionMaps =
                    loadMaterialTextures( material,
                                          aiTextureType_EMISSIVE,
                                          emission);

    textures.insert(textures.end(),
                    emissionMaps.begin(),
                    emissionMaps.end());

  }

  //----------------------------------------------------------------------------------- draw mode
  GLenum drawMode = GL_TRIANGLES;

  if ((mesh->mPrimitiveTypes & aiPrimitiveType_POINT) != 0)
  {
      drawMode = GL_POINTS;
  }
  else if ((mesh->mPrimitiveTypes & aiPrimitiveType_LINE) != 0)
  {
      drawMode = GL_LINES;
  }
  else if ((mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) != 0)
  {
      drawMode = GL_TRIANGLES;
  }
  //----------------------------------------------------------------------------------- create mesh
  
  return Mesh(vertices, indices, textures, drawMode);

}

std::vector<MeshTexture> Model::loadMaterialTextures( aiMaterial    *material, 
                                                      aiTextureType assimpType, 
                                                      TextureType   textureType){
  std::vector<MeshTexture> materialTextures;

  const unsigned int textureCount = material->GetTextureCount(assimpType);

  materialTextures.reserve(textureCount);

  for (unsigned int i = 0; i < textureCount; ++i)
  {
    aiString relativePath;

    if (material->GetTexture(assimpType, i, &relativePath) != AI_SUCCESS)
      continue;

    //---------------------------------------------------------------------------------- full path
    const std::string fullPath = directory + "/" + relativePath.C_Str();
    
    //----------------------------------------------------------------------------- cached texture
    auto existingTexture = loadedTextures.find(fullPath);

    if (existingTexture != loadedTextures.end()){
      materialTextures.push_back(MeshTexture{existingTexture->second, textureType});
      continue;
    }
    //-------------------------------------------------------------------------------- new texture
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(fullPath, true);

    loadedTextures.emplace(fullPath, texture);
    materialTextures.push_back(MeshTexture{texture, textureType});

  }
  return materialTextures;
}
