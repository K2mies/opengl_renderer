#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>

#include "Mat4.h"
#include "Mat3.h"
#include "Vec4.h"
#include "Vec3.h"
#include "MathUtils.h"

enum Camera_Movement{
  
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


const float YAW           = -90.0f;
const float PITCH         =  0.0f;
const float SPEED         =  2.5f;
const float SENSITIVITY   =  0.1f;
const float ZOOM          =  45.0f;

class Camera{

public:

  vec3  position;
  vec3  front;
  vec3  up;
  vec3  right;
  vec3  worldup;

  float yaw;
  float pitch;
  float roll;

  float speed;
  float sensitivity;
  float zoom;

  Camera  ( const vec3  &position,    
            const vec3  &up,    
            float yaw, 
            float pitch );

  Camera  ( float position[3], 
            float up[3], 
            float yaw,
            float pitch );

  mat4    getViewMatrix();

  void    processKeyboard       (Camera_Movement direction, float deltaTime);

  void    processMouseMovement  (float offset[2], GLboolean constrainPitch = true);

  void    processMouseScroll    (float offset[2] );

private:

  void updateCameraVectors();



};

#endif
