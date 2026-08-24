#include "MathUtils.h"

#include <cmath>

float math::radians(float degrees)
{
    return degrees * PI / 180.0f;
}

float math::degrees(float radians)
{
    return radians * 180.0f / PI;
}

float math::sqrt(float value)
{
    return std::sqrt(value);
}
