#ifndef VERTEX_H
#define VERTEX_H

#include "Typedefs/Vec2.h"
#include "Typedefs/Vec3.h"
#include "Typedefs/Vec4.h"

struct  Vertex
{
        vec3  position;
        vec3  normal;
        vec2  texCoords;

        vec3  tangent;
        vec3  bitangent;

        vec4  color;
};

#endif
