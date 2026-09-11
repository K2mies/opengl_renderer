#pragma once

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "Typedefs/Mat4.h"
#include "Typedefs/Mat3.h"
#include "Typedefs/Mat2.h"
#include "Typedefs/Vec4.h"
#include "Typedefs/Vec3.h"
#include "Typedefs/Vec2.h"

#include "Objects/Texture.h"

struct  Weight
{
        float   texture;
        float   vertex;
        float   location;
        float   blend;
};

struct  Euler
{
        float   pitch;
        float   yaw;
        float   roll;
};

struct  Frustum
{
        float   left;
        float   right;
        float   bottom;
        float   top;
        float   near;
        float   far;

};

struct  Matrix
{
        mat4    local;
        mat4    model;
        mat4    view;
        mat4    projection;
        mat4    clip;
        mat3    normal;

};

struct  Time
{
        float   delta_time;
        float   last_frame;
        float   current_frame;
};

struct  Color
{
        vec3    object;
        vec3    light;
};

struct  Material
{
        Texture diffuse;
        Texture specular;
        Texture emission;

        float   shininess;
};

struct  Light
{
        vec4    position;
        vec4    direction;

        vec3    ambient;
        vec3    diffuse;
        vec3    specular;

        float   constant;
        float   linear;
        float   quadratic;

        float   cutoff;
        float   outer_cutoff;
};

struct  SunLight {

        vec4     direction;
        
        vec3     ambient;
        vec3     diffuse;
        vec3     specular;

};

struct  PointLight {
        
        vec4     position;
    
        float    constant;
        float    linear;
        float    quadratic;
        
        vec3     ambient;
        vec3     diffuse;
        vec3     specular;
};

struct  SpotLight
{
        vec4    position;
        vec4    direction;

        vec3    ambient;
        vec3    diffuse;
        vec3    specular;

        float   constant;
        float   linear;
        float   quadratic;

        float   cutoff;
        float   outer_cutoff;
};
