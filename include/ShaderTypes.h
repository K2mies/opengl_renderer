#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Weight
{
    float texture;
    float vertex;
    float location;
    float blend;
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
    glm::mat4 local;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 clip;

};
