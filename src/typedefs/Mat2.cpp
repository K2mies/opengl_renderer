#include "Mat2.h"

#include <stdexcept>
#include <iomanip>
#include <cmath>

//--------------------------------------------------------- constructors

mat2::mat2()
{
    column[0] = vec2(1.0f, 0.0f);
    column[1] = vec2(0.0f, 1.0f);
}

mat2::mat2(float diagonal)
{
    column[0] = vec2(diagonal, 0.0f);
    column[1] = vec2(0.0f, diagonal);
}

mat2::mat2(const vec2& c0,
           const vec2& c1)
{
    column[0] = c0;
    column[1] = c1;
}

mat2::mat2(const mat2& source)
{
    *this = source;
}

//------------------------------------------------------------ operators

mat2& mat2::operator = (const mat2& source)
{
    if (this != &source)
    {
        column[0] = source.column[0];
        column[1] = source.column[1];
    }

    return *this;
}

vec2& mat2::operator [] (int index)
{
    if (index < 0 || index > 1)
        throw std::out_of_range("mat2 index out of range");

    return column[index];
}

const vec2& mat2::operator [] (int index) const
{
    if (index < 0 || index > 1)
        throw std::out_of_range("mat2 index out of range");

    return column[index];
}

mat2 mat2::operator + (const mat2& other) const
{
    mat2 temp(*this);

    temp.column[0] += other.column[0];
    temp.column[1] += other.column[1];

    return temp;
}

mat2 mat2::operator - (const mat2& other) const
{
    mat2 temp(*this);

    temp.column[0] -= other.column[0];
    temp.column[1] -= other.column[1];

    return temp;
}

mat2& mat2::operator += (const mat2& other)
{
    column[0] += other.column[0];
    column[1] += other.column[1];

    return *this;
}

mat2& mat2::operator -= (const mat2& other)
{
    column[0] -= other.column[0];
    column[1] -= other.column[1];

    return *this;
}

mat2 mat2::operator * (float value) const
{
    mat2 temp(*this);

    temp.column[0] *= value;
    temp.column[1] *= value;

    return temp;
}

mat2& mat2::operator *= (float value)
{
    column[0] *= value;
    column[1] *= value;

    return *this;
}

vec2 mat2::operator * (const vec2& vector) const
{
    vec2 result;

    result.x =
          column[0].x * vector.x
        + column[1].x * vector.y;

    result.y =
          column[0].y * vector.x
        + column[1].y * vector.y;

    return result;
}

mat2 mat2::operator * (const mat2& other) const
{
    return mat2(
        *this * other.column[0],
        *this * other.column[1]
    );
}

//---------------------------------------------------- utility functions
float* mat2::data()
{
    return &column[0].x;
}

const float* mat2::data() const
{
    return &column[0].x;
}

mat2 mat2::identity()
{
    return mat2(1.0f);
}

mat2 mat2::scale(const vec2& scale)
{
    mat2 result(1.0f);

    result[0].x = scale.x;
    result[1].y = scale.y;

    return result;
}

mat2 mat2::rotate(float angle)
{
    mat2 result(1.0f);

    float c = std::cos(angle);
    float s = std::sin(angle);

    result[0].x =  c;
    result[0].y =  s;

    result[1].x = -s;
    result[1].y =  c;

    return result;
}

mat2 mat2::transpose() const
{
    mat2 result;

    result[0].x = column[0].x;
    result[0].y = column[1].x;

    result[1].x = column[0].y;
    result[1].y = column[1].y;

    return result;
}

mat2 mat2::transpose(const mat2& matrix)
{
    return matrix.transpose();
}

float mat2::determinant() const
{
    return column[0].x * column[1].y
         - column[1].x * column[0].y;
}

float mat2::determinant(const mat2& matrix)
{
    return matrix.determinant();
}

mat2 mat2::inverse() const
{
    float det = determinant();

    if (det == 0.0f)
        throw std::runtime_error("mat2 cannot invert singular matrix");

    float invDet = 1.0f / det;

    mat2 result;

    result[0].x =  column[1].y * invDet;
    result[0].y = -column[0].y * invDet;

    result[1].x = -column[1].x * invDet;
    result[1].y =  column[0].x * invDet;

    return result;
}

mat2 mat2::inverse(const mat2& matrix)
{
    return matrix.inverse();
}

//------------------------------------------------- non-member functions

mat2 operator * (float value, const mat2& matrix)
{
    return matrix * value;
}

std::ostream& operator << (std::ostream& out, const mat2& obj)
{
    out << std::fixed << std::setprecision(3);

    out << "{\n";

    out << "  {"
        << std::setw(8) << obj[0].x << ", "
        << std::setw(8) << obj[1].x
        << "},\n";

    out << "  {"
        << std::setw(8) << obj[0].y << ", "
        << std::setw(8) << obj[1].y
        << "}\n";

    out << "}";

    return out;
}
