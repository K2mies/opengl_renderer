#include "Mat4.h"

#include <stdexcept>
#include <iomanip>
#include <cmath>

enum AxisType {

  x = 0,
  y = 1,
  z = 2,
  w = 3

};

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

const float*  mat4::data() const{
  
    return &column[0].x;
}

mat4 mat4::identity(){

    return mat4(1.0f);

}

mat4 mat4::translate    ( const vec3& offset ){

   mat4 result(1.0f);

    result[3] = vec4(offset, 1.0f);

    return result;

}

mat4 mat4::scale        ( const vec3& scale ){

    mat4 result(1.0f);

    result[0].x = scale.x;
    result[1].y = scale.y;
    result[2].z = scale.z;

    return result;

}

mat4 mat4::scale        ( const float scale[3] ){

    mat4 result(1.0f);

    result[0].x = scale[x];
    result[1].y = scale[y];
    result[2].z = scale[z];

    return result;

}

mat4 mat4::scale        ( const float scale ){

    mat4 result(1.0f);

    result[0].x = scale;
    result[1].y = scale;
    result[2].z = scale;

    return result;

}

mat4 mat4::scale        ( const float scale_x, const float scale_y, const float scale_z ){

    mat4 result(1.0f);

    result[0].x = scale_x;
    result[1].y = scale_y;
    result[2].z = scale_z;

    return result;

}


mat4 mat4::rotateX ( float angle ){

  mat4  result(1.0f);

  float c = std::cos(angle);
  float s = std::sin(angle);

  result[1].y =  c;
  result[1].z =  s;

  result[2].y = -s;
  result[2].z =  c;

  return result;
}

mat4 mat4::rotateY ( float angle ){

  mat4 result(1.0f);

    float c = std::cos(angle);
    float s = std::sin(angle);

    result[1].y =  c;
    result[1].z =  s;

    result[2].y = -s;
    result[2].z =  c;

    return result;

}

mat4 mat4::rotateZ ( float angle ){

   mat4 result(1.0f);

    float c = std::cos(angle);
    float s = std::sin(angle);

    result[0].x =  c;
    result[0].z = -s;

    result[2].x =  s;
    result[2].z =  c;

    return result;

}

mat4  mat4::rotateXYZ   ( float angles[3] ){

  mat4  result;

  result = result * mat4::rotateX( angles[x] );
  result = result * mat4::rotateY( angles[y] );
  result = result * mat4::rotateZ( angles[z] );

  return result;
}

mat4  mat4::rotate  ( float angle, const vec3& axis  ){

    vec3 a = axis.normalized();

    float c = std::cos(angle);
    float s = std::sin(angle);
    float t = 1.0f - c;

    mat4 result(1.0f);

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

mat4 mat4::perspective  (  float fov,
                           float aspect,
                           float nearPlane,
                           float farPlane    ){

    mat4  result(0.0f);

    float tanHalfFov = std::tan(  fov / 2.0f  );

    result[0].x = 1.0f / (  aspect * tanHalfFov );
    result[1].y = 1.0f /             tanHalfFov;

    result[2].z = -(  farPlane + nearPlane  )
                 / (  farPlane - nearPlane  );

    result[2].w = -1.0f;

    result[3].z = -(  2.0f * farPlane * nearPlane )
                 / (         farPlane - nearPlane  );

    return result;
}

mat4 mat4::ortho(
    float left,
    float right,
    float bottom,
    float top,
    float nearPlane,
    float farPlane
)
{
    mat4 result(1.0f);

    result[0].x =  2.0f / (right    - left);
    result[1].y =  2.0f / (top      - bottom);
    result[2].z = -2.0f / (farPlane - nearPlane);

    result[3].x =        -(right + left) 
                        / (right - left);

    result[3].y =        -(top + bottom) 
                        / (top - bottom);

    result[3].z =        -(farPlane + nearPlane) 
                        / (farPlane - nearPlane);

    return result;
}

mat4 mat4::lookAt(const vec3& eye,
                  const vec3& center,
                  const vec3& up)
{
    vec3 forward  = vec3::normalized  (center - eye);
    vec3 right    = vec3::cross       (forward, up).normalized();
    vec3 cameraUp = vec3::cross       (right, forward);

    mat4 result(1.0f);

    result[0].x =  right.x;
    result[1].x =  right.y;
    result[2].x =  right.z;

    result[0].y =  cameraUp.x;
    result[1].y =  cameraUp.y;
    result[2].y =  cameraUp.z;

    result[0].z = -forward.x;
    result[1].z = -forward.y;
    result[2].z = -forward.z;

    result[3].x = -right.dot    (eye);
    result[3].y = -cameraUp.dot (eye);
    result[3].z =  forward.dot  (eye);
  
    return result;
}

mat4 mat4::transpose() const
{
    mat4 result;

    result[0].x = column[0].x;
    result[0].y = column[1].x;
    result[0].z = column[2].x;
    result[0].w = column[3].x;

    result[1].x = column[0].y;
    result[1].y = column[1].y;
    result[1].z = column[2].y;
    result[1].w = column[3].y;

    result[2].x = column[0].z;
    result[2].y = column[1].z;
    result[2].z = column[2].z;
    result[2].w = column[3].z;

    result[3].x = column[0].w;
    result[3].y = column[1].w;
    result[3].z = column[2].w;
    result[3].w = column[3].w;

    return result;
}

mat4 mat4::transpose(const mat4& matrix)
{
    return matrix.transpose();
}

float mat4::determinant() const
{
    return

          column[0].x *
        (
              column[1].y *
            (
                  column[2].z * column[3].w
                - column[3].z * column[2].w
            )
            - column[2].y *
            (
                  column[1].z * column[3].w
                - column[3].z * column[1].w
            )
            + column[3].y *
            (
                  column[1].z * column[2].w
                - column[2].z * column[1].w
            )
        )
        - column[1].x *
        (
              column[0].y *
            (
                  column[2].z * column[3].w
                - column[3].z * column[2].w
            )
            - column[2].y *
            (
                  column[0].z * column[3].w
                - column[3].z * column[0].w
            )
            + column[3].y *
            (
                  column[0].z * column[2].w
                - column[2].z * column[0].w
            )
        )
        + column[2].x *
        (
              column[0].y *
            (
                  column[1].z * column[3].w
                - column[3].z * column[1].w
            )
            - column[1].y *
            (
                  column[0].z * column[3].w
                - column[3].z * column[0].w
            )
            + column[3].y *
            (
                  column[0].z * column[1].w
                - column[1].z * column[0].w
            )
        )
        - column[3].x *
        (
              column[0].y *
            (
                  column[1].z * column[2].w
                - column[2].z * column[1].w
            )
            - column[1].y *
            (
                  column[0].z * column[2].w
                - column[2].z * column[0].w
            )
            + column[2].y *
            (
                  column[0].z * column[1].w
                - column[1].z * column[0].w
            )
        );
}

float mat4::determinant(const mat4& matrix)
{
    return matrix.determinant();
}

mat4 mat4::inverse() const
{
    float coef00 =
          column[2].z * column[3].w
        - column[3].z * column[2].w;

    float coef02 =
          column[1].z * column[3].w
        - column[3].z * column[1].w;

    float coef03 =
          column[1].z * column[2].w
        - column[2].z * column[1].w;

    float coef04 =
          column[2].y * column[3].w
        - column[3].y * column[2].w;

    float coef06 =
          column[1].y * column[3].w
        - column[3].y * column[1].w;

    float coef07 =
          column[1].y * column[2].w
        - column[2].y * column[1].w;

    float coef08 =
          column[2].y * column[3].z
        - column[3].y * column[2].z;

    float coef10 =
          column[1].y * column[3].z
        - column[3].y * column[1].z;

    float coef11 =
          column[1].y * column[2].z
        - column[2].y * column[1].z;

    float coef12 =
          column[2].x * column[3].w
        - column[3].x * column[2].w;

    float coef14 =
          column[1].x * column[3].w
        - column[3].x * column[1].w;

    float coef15 =
          column[1].x * column[2].w
        - column[2].x * column[1].w;

    float coef16 =
          column[2].x * column[3].z
        - column[3].x * column[2].z;

    float coef18 =
          column[1].x * column[3].z
        - column[3].x * column[1].z;

    float coef19 =
          column[1].x * column[2].z
        - column[2].x * column[1].z;

    float coef20 =
          column[2].x * column[3].y
        - column[3].x * column[2].y;

    float coef22 =
          column[1].x * column[3].y
        - column[3].x * column[1].y;

    float coef23 =
          column[1].x * column[2].y
        - column[2].x * column[1].y;

    vec4 fac0(
        coef00,
        coef00,
        coef02,
        coef03
    );

    vec4 fac1(
        coef04,
        coef04,
        coef06,
        coef07
    );

    vec4 fac2(
        coef08,
        coef08,
        coef10,
        coef11
    );

    vec4 fac3(
        coef12,
        coef12,
        coef14,
        coef15
    );

    vec4 fac4(
        coef16,
        coef16,
        coef18,
        coef19
    );

    vec4 fac5(
        coef20,
        coef20,
        coef22,
        coef23
    );

    vec4 vec0(
        column[1].x,
        column[0].x,
        column[0].x,
        column[0].x
    );

    vec4 vec1(
        column[1].y,
        column[0].y,
        column[0].y,
        column[0].y
    );

    vec4 vec2(
        column[1].z,
        column[0].z,
        column[0].z,
        column[0].z
    );

    vec4 vec3(
        column[1].w,
        column[0].w,
        column[0].w,
        column[0].w
    );

    vec4 inv0 =
          vec1 * fac0
        - vec2 * fac1
        + vec3 * fac2;

    vec4 inv1 =
          vec0 * fac0
        - vec2 * fac3
        + vec3 * fac4;

    vec4 inv2 =
          vec0 * fac1
        - vec1 * fac3
        + vec3 * fac5;

    vec4 inv3 =
          vec0 * fac2
        - vec1 * fac4
        + vec2 * fac5;

    vec4 signA(
         1.0f,
        -1.0f,
         1.0f,
        -1.0f
    );

    vec4 signB(
        -1.0f,
         1.0f,
        -1.0f,
         1.0f
    );

    mat4 result(
        inv0 * signA,
        inv1 * signB,
        inv2 * signA,
        inv3 * signB
    );

    vec4 row0(
        result[0].x,
        result[1].x,
        result[2].x,
        result[3].x
    );

    float determinant =
        column[0].x * row0.x
      + column[0].y * row0.y
      + column[0].z * row0.z
      + column[0].w * row0.w;

    if (determinant == 0.0f)
        throw std::runtime_error("mat4 cannot invert singular matrix");

    float inverseDeterminant = 1.0f / determinant;

    result[0] *= inverseDeterminant;
    result[1] *= inverseDeterminant;
    result[2] *= inverseDeterminant;
    result[3] *= inverseDeterminant;

    return result;
}

mat4 mat4::inverse(const mat4& matrix)
{
    return matrix.inverse();
}

mat3 mat4::upperLeft3x3() const
{
    return mat3(
        vec3(column[0].x, column[0].y, column[0].z),
        vec3(column[1].x, column[1].y, column[1].z),
        vec3(column[2].x, column[2].y, column[2].z)
    );
}

mat3 mat4::upperLeft3x3(const mat4& matrix)
{
    return mat3(

           vec3(matrix.column[0].x,
                matrix.column[0].y,
                matrix.column[0].z),

           vec3(matrix.column[1].x,
                matrix.column[1].y,
                matrix.column[1].z),

           vec3(matrix.column[2].x,
                matrix.column[2].y,
                matrix.column[2].z)
    );
}

mat3 mat4::normalMatrix() const
{
    mat3 normal;

    normal = upperLeft3x3();
    normal = normal.inverse();
    normal = normal.transpose();

    return normal;
}

mat3 mat4::normalMatrix(const mat4& matrix)
{
    
    mat3 normal;

    normal = matrix.upperLeft3x3();
    normal = normal.inverse();
    normal = normal.transpose();

    return normal;
}

//--------------------------------------------------------- non-member functions 
std::ostream& operator<<(std::ostream& out, const mat4& obj)
{
    out << std::fixed << std::setprecision(3);

    out << "{\n";

    out << "  {"
        << std::setw(8) << obj[0].x << ", "
        << std::setw(8) << obj[1].x << ", "
        << std::setw(8) << obj[2].x << ", "
        << std::setw(8) << obj[3].x
        << "},\n";

    out << "  {"
        << std::setw(8) << obj[0].y << ", "
        << std::setw(8) << obj[1].y << ", "
        << std::setw(8) << obj[2].y << ", "
        << std::setw(8) << obj[3].y
        << "},\n";

    out << "  {"
        << std::setw(8) << obj[0].z << ", "
        << std::setw(8) << obj[1].z << ", "
        << std::setw(8) << obj[2].z << ", "
        << std::setw(8) << obj[3].z
        << "},\n";

    out << "  {"
        << std::setw(8) << obj[0].w << ", "
        << std::setw(8) << obj[1].w << ", "
        << std::setw(8) << obj[2].w << ", "
        << std::setw(8) << obj[3].w
        << "}\n";

    out << "}";

    return out;
}
