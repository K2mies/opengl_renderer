#include "Mat3.h"

#include <stdexcept>
#include <iomanip>
#include <cmath>

//--------------------------------------------------------- constructors
mat3::mat3(){

  column[0] = vec3(1.0f, 0.0f, 0.0f);
  column[1] = vec3(0.0f, 1.0f, 0.0f);
  column[2] = vec3(0.0f, 0.0f, 1.0f);

}

mat3::mat3    ( float diagonal  ){

  column[0] = vec3(diagonal, 0.0f,     0.0f    );
  column[1] = vec3(0.0f,     diagonal, 0.0f    );
  column[2] = vec3(0.0f,     0.0f,     diagonal);

}

mat3::mat3    ( const vec3& c0,
                const vec3& c1,
                const vec3& c2 ) {

                column[0] = c0;
                column[1] = c1;
                column[2] = c2;

}

mat3::mat3    ( const mat3& source ){
  *this = source;
}

//------------------------------------------------------------ operators

mat3& mat3::operator  =   ( const mat3&   source  ){
   
  if (this != &source)
    {
        column[0] = source.column[0];
        column[1] = source.column[1];
        column[2] = source.column[2];
    }

    return *this;

}

vec3& mat3::operator  []  (int index){

  if (index < 0 || index > 2)
        throw std::out_of_range("mat3 index out of range");

    return column[index];

}
const vec3& mat3::operator  []  (int index) const{

  if (index < 0 || index > 2)
        throw std::out_of_range("mat3 index out of range");

    return column[index];
}

mat3  mat3::operator + ( const mat3& other ) const{
   
  mat3 temp(*this);

    temp.column[0] += other.column[0];
    temp.column[1] += other.column[1];
    temp.column[2] += other.column[2];

    return temp;

}

mat3  mat3::operator - ( const mat3& other ) const{

  mat3 temp(*this);

    temp.column[0] -= other.column[0];
    temp.column[1] -= other.column[1];
    temp.column[2] -= other.column[2];

    return temp;

}

mat3& mat3::operator += ( const mat3& other ){

    column[0] += other.column[0];
    column[1] += other.column[1];
    column[2] += other.column[2];

    return *this;

}

mat3& mat3::operator -= ( const mat3& other ){

    column[0] -= other.column[0];
    column[1] -= other.column[1];
    column[2] -= other.column[2];

    return *this;

}

mat3 mat3::operator * ( float value) const{

   mat3 temp(*this);

    temp.column[0] *= value;
    temp.column[1] *= value;
    temp.column[2] *= value;

    return temp;

}

mat3& mat3::operator *= ( float value ){

    column[0] *= value;
    column[1] *= value;
    column[2] *= value;

    return *this;

}  

vec3  mat3::operator * ( const vec3& vector ) const{

   vec3 result;

    result.x =
          column[0].x * vector.x
        + column[1].x * vector.y
        + column[2].x * vector.z;

    result.y =
          column[0].y * vector.x
        + column[1].y * vector.y
        + column[2].y * vector.z;

    result.z =
          column[0].z * vector.x
        + column[1].z * vector.y
        + column[2].z * vector.z;

    return result;

}
mat3  mat3::operator * ( const mat3& other ) const{

    return mat3(
        *this * other.column[0],
        *this * other.column[1],
        *this * other.column[2]
    );

}

//---------------------------------------------------- utility functions

float* mat3::data()
{
    return &column[0].x;
}

const float*  mat3::data()                         const{
  
  return &column[0].x;
}

mat3 mat3::identity(){

  return mat3(1.0f);

}

mat3 mat3::scale        ( const vec3& scale ){

    mat3 result(1.0f);

    result[0].x = scale.x;
    result[1].y = scale.y;
    result[2].z = scale.z;

    return result;

}

mat3 mat3::rotateX ( float angle ){

    float c = std::cos(angle);
    float s = std::sin(angle);

    mat3 result(1.0f);

    result[1].y =  c;
    result[1].z =  s;

    result[2].y = -s;
    result[2].z =  c;

    return result;

}

mat3 mat3::rotateY ( float angle ){

    float c = std::cos(angle);
    float s = std::sin(angle);

    mat3 result(1.0f);

    result[0].x =  c;
    result[0].z = -s;

    result[2].x =  s;
    result[2].z =  c;

    return result;

}

mat3 mat3::rotateZ ( float angle ){
 
    float c = std::cos(angle);
    float s = std::sin(angle);

    mat3 result(1.0f);

    result[0].x =  c;
    result[0].z =  s;

    result[1].x = -s;
    result[1].z =  c;

    return result;

}

mat3 mat3::rotate(float angle, const vec3& axis)
{
    vec3 a = axis.normalized();

    float c = std::cos(angle);
    float s = std::sin(angle);
    float t = 1.0f - c;

    mat3 result(1.0f);

    result[0].x = t * a.x * a.x + c;
    result[0].y = t * a.x * a.y + s * a.z;
    result[0].z = t * a.x * a.z - s * a.y;

    result[1].x = t * a.x * a.y - s * a.z;
    result[1].y = t * a.y * a.y + c;
    result[1].z = t * a.y * a.z + s * a.x;

    result[2].x = t * a.x * a.z + s * a.y;
    result[2].y = t * a.y * a.z - s * a.x;
    result[2].z = t * a.z * a.z + c;

    return result;
}

//--------------------------------------------------------- non-member functions 
mat3 operator*(float num, const mat3& obj)
{
    mat3 temp(obj);

    temp *= num;

    return temp;
}

std::ostream& operator<<(std::ostream& out,
                         const mat3& obj)
{
    out << "{\n";

    for (int row = 0; row < 3; row++)
    {
        out << "  {";

        for (int col = 0; col < 3; col++)
        {
            out << std::setw(8)
                << std::fixed
                << std::setprecision(3)
                << obj[col][row];

            if (col < 2)
                out << ", ";
        }

        out << "}";

        if (row < 2)
            out << ",";

        out << "\n";
    }

    out << "}";

    return out;
}
