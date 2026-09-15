#ifndef MODEL_H
#define MODEL_H

#include <assimp/material.h>
#include <assimp/scene.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Objects/Mesh.h"
#include "Objects/Shader.h"
#include "Typedefs/MeshTexture.h"

class Model
{
public:
        explicit Model    (const std::string &path);

        void     draw     (Shader &shader);

private:
        std::vector<Mesh> meshes;
        std::string       directory;

        std::unordered_map< std::string, std::shared_ptr<Texture> >  loadedTextures;

        void              loadModel   (const std::string &path);
        void              processNode (aiNode *node, const aiScene *scene);
        Mesh              processMesh (aiMesh *mesh, const aiScene *scene);

        std::vector<MeshTexture> loadMaterialTextures(
                          aiMaterial   *material, 
                          aiTextureType assimpType, 
                          TextureType   TextureType);
};

#endif
