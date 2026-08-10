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

vec4&            mat4::operator  []  (int index){

  if (index < 0 || index > 3)
        throw std::out_of_range("mat4 index out of range");

    return column[index];

}
const   vec4&     mat4::operator  []  (int index)         const{

  if (index < 0 || index > 3)
        throw std::out_of_range("mat4 index out of range");

    return column[index];
}

//mat4    operator  +   ( const mat4& )         const;
//mat4    operator  -   ( const mat4& )         const;
//
//mat4&   operator  +=  ( const mat4& );
//mat4&   operator  -=  ( const mat4& );
//
//mat4    operator  *   ( float )               const;
//mat4&   operator  *=  ( float );  
//
//vec4    operator  *   ( const vec4& )         const;
//mat4    operator  *   ( const mat4& )         const;

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
