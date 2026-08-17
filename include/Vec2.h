#ifndef VEC2_H
#define VEC2_H

#include <iostream>
#include <cmath>

class vec2
{
public:

    //------------------------------------------------ variables

    float x;
    float y;

    //--------------------------------------------- constructors

    vec2();
    vec2(float x, float y);
    vec2(const vec2& source);

    //------------------------------------------------ operators

    vec2&   operator  =   (const vec2& source);

    float   operator  []  (int index)             const;
    float&  operator  []  (int index);

    vec2    operator  -   ()                      const;

    vec2    operator  *   (float value)           const;
    vec2    operator  +   (const vec2& other)     const;
    vec2    operator  -   (const vec2& other)     const;
    vec2    operator  *   (const vec2& other)     const;

    vec2&   operator  *=  (float value);
    vec2&   operator  +=  (const vec2& other);
    vec2&   operator  -=  (const vec2& other);
    vec2&   operator  *=  (const vec2& other);

    vec2&   operator  ++  ();
    vec2    operator  ++  (int);

    vec2&   operator  --  ();
    vec2    operator  --  (int);

    bool    operator  ==  (const vec2& other)     const;
    bool    operator  !=  (const vec2& other)     const;

    //-------------------------------------------------- utility
    float        length()                         const;
    float        lengthSquared()                  const;

    void         normalize();

    vec2         normalized()                     const;
    static  vec2 normalized (const vec2& vector);

    float        dot        (const vec2& other)   const;
    static float dot        (const vec2& lhs,
                             const vec2& rhs);
};

vec2 operator*(int value, const vec2& vector);

std::ostream& operator<<(std::ostream& out, const vec2& vector);

#endif
