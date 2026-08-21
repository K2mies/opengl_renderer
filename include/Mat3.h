#ifndef MAT3_HPP
#define MAT3_HPP

#include <iostream>

#include "Vec3.h"

class mat3
{
public:

    vec3 column[3];

    float* data();
    const float* data() const;

	  //--------------------------------------------------------- constructors

    mat3();

    mat3(float diagonal);

    mat3(
        const vec3& c0,
        const vec3& c1,
        const vec3& c2
    );

    mat3  (const mat3&);

	  //------------------------------------------------------------ operators

    mat3&       operator  = (const mat3&);

    vec3&       operator  []  (int);

    const vec3& operator  [] (int)                                   const;

    mat3        operator  +  (const mat3&)                           const;

    mat3        operator  -  (const mat3&)                           const;

    mat3&       operator  += (const mat3&);

    mat3&       operator  -= (const mat3&);

    mat3        operator  *  (float)                                 const;

    mat3&       operator  *= (float);

    vec3        operator  *  (const vec3&)                           const;

    mat3        operator  *  (const mat3&)                           const;

	  //---------------------------------------------------- utility functions
    
    static mat3 identity      ();

    static mat3 scale         (const vec3&);

    static mat3 rotateX       (float);

    static mat3 rotateY       (float);

    static mat3 rotateZ       (float);

    static mat3 rotateXYZ     (float angles[3]);
    static mat3 rotate        (float angle, const vec3& axis);

    mat3        transpose     ()                                    const;
    static mat3 transpose     (const mat3& matrix);
    
    float determinant         ()                                    const;
    static float determinant  (const mat3& matrix);
    
    mat3 inverse              ()                                    const;
    static mat3 inverse       (const mat3& matrix);

    ~mat3() = default;

};

//--------------------------------------------------------- non-member functions 
mat3 operator*(float num, const mat3& obj);

std::ostream& operator<<(std::ostream& out, const mat3& obj);

#endif
