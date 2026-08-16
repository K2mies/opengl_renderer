#include "Camera.h"

enum AxisType {

  x = 0,
  y = 1,
  z = 2,
  w = 3

};

Camera::Camera            ( const vec3 &position, 
                            const vec3 &up, 
                            float yaw, 
                            float pitch ){

  this->position = position;
  this->worldup = up;
  
  this->yaw     = yaw;
  this->pitch   = pitch;
  this->roll    = 0.0f;

  speed         =  2.5f;
  sensitivity   =  0.1f;
  zoom          = 45.0f;

  updateCameraVectors();

}

Camera::Camera             ( float position[3],
                             float up[3],
                             float yaw,
                             float pitch )
{
    this->position = vec3 (  position[x],
                             position[y], 
                             position[z] );

    this->worldup  = vec3(   up[x],
                             up[y],
                             up[z]);

    this->yaw   = yaw;
    this->pitch = pitch;
    this->roll  = 0.0f;

    speed       = SPEED;
    sensitivity = SENSITIVITY;
    zoom        = ZOOM;

    updateCameraVectors();
}

mat4 Camera::getViewMatrix(){

  return mat4::lookAt ( position,
                        position 
                      + front,
                        up );
}

void Camera::processKeyboard( Camera_Movement direction, float deltaTime  ){

  float velocity = speed * deltaTime;

  if (direction == FORWARD)
    position += front * velocity;

  if (direction == BACKWARD)
    position -= front * velocity;

  if (direction == LEFT)
    position -= right * velocity;

  if (direction == RIGHT)
    position += right * velocity;
}

void Camera::processMouseMovement(float offset[2], GLboolean constrainPitch){

  yaw   += offset[x] * sensitivity;
  pitch += offset[y] * sensitivity;

  if  (constrainPitch){
    if (pitch >= 89.0f)
        pitch =  89.0f;

    if (pitch <=-89.0f)
        pitch = -89.0f;
  }
  
  updateCameraVectors();
}

void Camera::processMouseScroll(float offset[2]){
  
  zoom -= offset[y];

  if (zoom <= 1.0f)
      zoom =  1.0f;

  if (zoom >= 45.0f)
      zoom =  45.0f;
}

void Camera::updateCameraVectors(){
  
  vec3 direction;

  direction.x = std::cos(math::radians( yaw   ))
              * std::cos(math::radians( pitch ));
  direction.y = std::sin(math::radians( pitch ));
  direction.z = std::sin(math::radians( yaw ))
              * std::cos(math::radians( pitch ));

  front       = direction.normalized();
  right       = vec3::cross(front, worldup).normalized();
  up          = vec3::cross(right, front).normalized();
}
