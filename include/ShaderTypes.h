#pragma once

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "Mat4.h"
#include "Mat3.h"
#include "Vec4.h"
#include "Vec3.h"
#include "Vec2.h"

struct Weight
{
    float texture;
    float vertex;
    float location;
    float blend;
};

struct  Euler
{
    float pitch;
    float yaw;
    float roll;
};

struct  Frustum
{
    float left;
    float right;
    float bottom;
    float top;
    float near;
    float far;

};

struct  Matrix
{
    mat4 local;
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 clip;

};

struct Time
{
  float delta_time;
  float last_frame;
  float current_frame;
};

struct Color
{
  vec3  object;
  vec3  light;
};
