#ifndef MESH_TEXTURE_H
#define MESH_TEXTURE_H

#include <memory>

#include "Objects/Texture.h"

enum  TextureType
{
      diffuse,
      specular,
      emission
};

struct MeshTexture
{

  std::shared_ptr<Texture> texture; //A shared_ptr<Texture> lets multiple MeshTextures safely refer to the same actual OpenGL texture without copying it:
  TextureType              type;

};

#endif
