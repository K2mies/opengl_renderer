#ifndef MAT4_HPP
#define MAT4_HPP

#include<iostream>

#include "Vec3.h"
#include "Vec4.h"

class mat4
{
public:

  vec4 column[4];

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

  float                  *data();

  const float*            data()                const;
  
  static  mat4            identity       ();
  static  mat4            translate      (  const vec3& );

  static  mat4            scale          (  const vec3& );
  static  mat4            scale          (  const float scale[3] );
  static  mat4            scale          (  const float scale    );
  static  mat4            scale          (  const float scale_x, 
                                            const float scale_y, 
                                            const float scale_z  );
  
  static  mat4            rotateX        (  float );
  static  mat4            rotateY        (  float );
  static  mat4            rotateZ        (  float );

  static  mat4            rotateXYZ      ( float angles[3]    );
  static  mat4            rotate         (  float angle, 
                                            const vec3& axis  );

  static  mat4            perspective    (  float fov,
                                            float aspect,
                                            float nearPlane,
                                            float farPlane    );

  static  mat4            ortho          (  float left,
                                            float right,
                                            float bottom,
                                            float top,
                                            float nearPlane,
                                            float farPlane    );

  static  mat4            lookAt         (  const vec3& eye,
                                            const vec3& center,
                                            const vec3& up     ); 

          mat4            transpose      () const;
  static  mat4            transpose      (  const mat4& matrix );

          float           determinant    () const;
  static  float           determinant    (  const mat4& matrix );

          mat4            inverse        () const;
  static  mat4            inverse        (  const mat4& matrix );

	//----------------------------------------------------------- destructor
	~mat4   () = default;
};

//--------------------------------------------------------- non-member functions 
std::ostream& operator<<(std::ostream& out, const mat4& obj);

#endif
