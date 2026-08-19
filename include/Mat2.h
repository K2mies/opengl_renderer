#ifndef MAT2_H
#define MAT2_H

#include "Vec2.h"

#include <iostream>

class mat2
{
public:

    //---------------------------------------------------------- variables

    vec2 column[2];

    //------------------------------------------------------- constructors

    mat2();

    mat2(float diagonal);

    mat2(const vec2& c0,
         const vec2& c1);

    mat2(const mat2& source);

    //---------------------------------------------------------- operators

    mat2& operator  = (const mat2& source);

    vec2&           operator [] (int index);
    const vec2&     operator [] (int index)             const;

    mat2 operator   + (const mat2& other)               const;
    mat2 operator   - (const mat2& other)               const;

    mat2& operator  += (const mat2& other);
    mat2& operator  -= (const mat2& other);

    mat2  operator  *  (float value)                    const;
    mat2& operator  *= (float value);

    vec2 operator   * (const vec2& vector)              const;

    mat2 operator   * (const mat2& other)               const;

    //------------------------------------------------ utility functions

    float*         data       ();
    const   float* data       ()                        const;

    static  mat2  identity    ();

    static  mat2  scale       (const vec2& scale);

    static  mat2  rotate      (float angle);

    mat2          transpose   ()                        const;

    static  mat2  transpose   (const mat2& matrix);

    float         determinant ()                        const;

    static  float determinant (const mat2& matrix);

    mat2          inverse     ()                        const;

    static  mat2  inverse     (const mat2& matrix);
};

//------------------------------------------------ non-member functions

mat2 operator * (float value, const mat2& matrix);

std::ostream& operator << (std::ostream& out, const mat2& obj);

#endif
