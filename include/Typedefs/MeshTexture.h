#ifndef MESH_TEXTURE_H
#define MESH_TEXTURE_H

#include <string>
#include <memory>

#include "Objects/Texture.h"

struct MeshTexture
{

  //const Texture *texture;
  std::shared_ptr<Texture> texture; //A shared_ptr<Texture> lets multiple MeshTextures safely refer to the same actual OpenGL texture without copying it:
  std::string   type;

};

#endif
