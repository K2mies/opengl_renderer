#ifndef MODEL_H
#define MODEL_H

#include <assimp/material.h>
#include <assimp/scene.h>

#include <string>
#include <vector>

#include "Objects/Mesh.h"
#include "Objects/Shader.h"

class Model
{
public:
        explicit Model    (const std::string &path);

        void     draw     (Shader &shader);

private:
        std::vector<Mesh> meshes;
        std::string       directory;

        void              loadModel   (const std::string &path);
        void              processNode (aiNode *node, const aiScene *scene);
        Mesh              processMesh (aiMesh *mesh, const aiScene *scene);
};

#endif
