#ifndef MAT4_HPP
#define MAT4_HPP

#include<iostream>

#include "Vec3.h"
#include "Vec4.h"

class mat4
{
public:

  vec4 column[4];

  float        *data();
  const float*  data()                         const;

	//--------------------------------------------------------- constructors
  mat4();

  mat4    ( float diagonal  );

  mat4    ( const vec4& c0,
            const vec4& c1,
            const vec4& c2,
            const vec4& c3  );

  mat4    ( const mat4& );

	//------------------------------------------------------------ operators
  
  mat4&   operator  =   ( const mat4&   source  );

  vec4&             operator  []  (int);
  const   vec4&     operator  []  (int)         const;
  
  mat4    operator  +   ( const mat4& )         const;
  mat4    operator  -   ( const mat4& )         const;

  mat4&   operator  +=  ( const mat4& );
  mat4&   operator  -=  ( const mat4& );

  mat4    operator  *   ( float )               const;
  mat4&   operator  *=  ( float );  

  vec4    operator  *   ( const vec4& )         const;
  mat4    operator  *   ( const mat4& )         const;

	//---------------------------------------------------- utility functions
  
  static  mat4            identity       ();

  static  mat4            translate      (const vec3&);
  
  static  mat4            scale          (const vec3&);
  
  static  mat4            rotateX        (float);
  
  static  mat4            rotateY        (float);
  
  static  mat4            rotateZ        (float);
  
  static  mat4            perspective    (...);
  
  static  mat4            ortho          (...);
  
  static  mat4            lookAt         (...);

	~mat4   () = default;
};

//--------------------------------------------------------- non-member functions 
std::ostream& operator<<(std::ostream& out, const mat4& obj);

#endif
