#ifndef MESH_TEXTURE_H
#define MESH_TEXTURE_H

#include <string>

#include "Objects/Texture.h"

struct MeshTexture
{

  const Texture *texture;
  std::string   type;

};

#endif
