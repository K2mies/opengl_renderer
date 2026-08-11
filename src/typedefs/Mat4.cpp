#include "Mat4.h"

#include <stdexcept>

//--------------------------------------------------------- constructors
mat4::mat4(){

  column[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
  column[1] = vec4(0.0f, 1.0f, 0.0f, 0.0f);
  column[2] = vec4(0.0f, 0.0f, 1.0f, 0.0f);
  column[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);

}

mat4::mat4    ( float diagonal  ){

  column[0] = vec4(diagonal, 0.0f,     0.0f,     0.0f);
  column[1] = vec4(0.0f,     diagonal, 0.0f,     0.0f);
  column[2] = vec4(0.0f,     0.0f,     diagonal, 0.0f);
  column[3] = vec4(0.0f,     0.0f,     0.0f,     diagonal);

}

mat4::mat4    ( const vec4& c0,
                const vec4& c1,
                const vec4& c2,
                const vec4& c3  ) {

                column[0] = c0;
                column[1] = c1;
                column[2] = c2;
                column[3] = c3;

}

mat4::mat4    ( const mat4& source ){
  *this = source;
}

//------------------------------------------------------------ operators

mat4& mat4::operator  =   ( const mat4&   source  ){
   
  if (this != &source)
    {
        column[0] = source.column[0];
        column[1] = source.column[1];
        column[2] = source.column[2];
        column[3] = source.column[3];
    }

    return *this;

}

vec4& mat4::operator  []  (int index){

  if (index < 0 || index > 3)
        throw std::out_of_range("mat4 index out of range");

    return column[index];

}
const vec4& mat4::operator  []  (int index) const{

  if (index < 0 || index > 3)
        throw std::out_of_range("mat4 index out of range");

    return column[index];
}

mat4  mat4::operator + ( const mat4& other ) const{
   
  mat4 temp(*this);

    temp.column[0] += other.column[0];
    temp.column[1] += other.column[1];
    temp.column[2] += other.column[2];
    temp.column[3] += other.column[3];

    return temp;

}

mat4  mat4::operator - ( const mat4& other ) const{

  mat4 temp(*this);

    temp.column[0] -= other.column[0];
    temp.column[1] -= other.column[1];
    temp.column[2] -= other.column[2];
    temp.column[3] -= other.column[3];

    return temp;

}

mat4& mat4::operator += ( const mat4& other ){

    column[0] += other.column[0];
    column[1] += other.column[1];
    column[2] += other.column[2];
    column[3] += other.column[3];

    return *this;

}

mat4& mat4::operator -= ( const mat4& other ){

    column[0] -= other.column[0];
    column[1] -= other.column[1];
    column[2] -= other.column[2];
    column[3] -= other.column[3];

    return *this;

}

mat4 mat4::operator * ( float value) const{

   mat4 temp(*this);

    temp.column[0] *= value;
    temp.column[1] *= value;
    temp.column[2] *= value;
    temp.column[3] *= value;

    return temp;

}

mat4& mat4::operator *= ( float value ){

    column[0] *= value;
    column[1] *= value;
    column[2] *= value;
    column[3] *= value;

    return *this;

}  

vec4  mat4::operator * ( const vec4& vector ) const{

  vec4 result;

    result.x =
          column[0].x * vector.x
        + column[1].x * vector.y
        + column[2].x * vector.z
        + column[3].x * vector.w;

    result.y =
          column[0].y * vector.x
        + column[1].y * vector.y
        + column[2].y * vector.z
        + column[3].y * vector.w;

    result.z =
          column[0].z * vector.x
        + column[1].z * vector.y
        + column[2].z * vector.z
        + column[3].z * vector.w;

    result.w =
          column[0].w * vector.x
        + column[1].w * vector.y
        + column[2].w * vector.z
        + column[3].w * vector.w;

    return result;

}
mat4  mat4::operator * ( const mat4& other ) const{

    return mat4(
        *this * other.column[0],
        *this * other.column[1],
        *this * other.column[2],
        *this * other.column[3]
    );

}

//---------------------------------------------------- utility functions

float* mat4::data()
{
    return &column[0].x;
}

const float*  mat4::data()                         const{
  
  return &column[0].x;
}

//static  mat4            identity       ();
//
//static  mat4            translate      (const vec3&);
//
//static  mat4            scale          (const vec3&);
//
//static  mat4            rotateX        (float);
//
//static  mat4            rotateY        (float);
//
//static  mat4            rotateZ        (float);
//
//static  mat4            perspective    (...);
//
//static  mat4            ortho          (...);
//
//static  mat4            lookAt         (...);


//--------------------------------------------------------- non-member functions 
std::ostream& operator<<(std::ostream& out, const mat4& obj){
  out << "{\n";

    out << "  {"
        << obj[0].x << ", "
        << obj[1].x << ", "
        << obj[2].x << ", "
        << obj[3].x
        << "},\n";

    out << "  {"
        << obj[0].y << ", "
        << obj[1].y << ", "
        << obj[2].y << ", "
        << obj[3].y
        << "},\n";

    out << "  {"
        << obj[0].z << ", "
        << obj[1].z << ", "
        << obj[2].z << ", "
        << obj[3].z
        << "},\n";

    out << "  {"
        << obj[0].w << ", "
        << obj[1].w << ", "
        << obj[2].w << ", "
        << obj[3].w
        << "}\n";

    out << "}";

    return out;
}
