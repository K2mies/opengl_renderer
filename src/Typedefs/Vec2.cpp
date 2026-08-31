#include "Vec2.h"

//----------------------------------------------------------------- constructors
vec2::vec2(){

  x = 0.0f;
  y = 0.0f;

}

vec2::vec2(float x, float y){
  
  this->x = x;
  this->y = y;

}

vec2::vec2(const vec2& source){
  
  *this = source;
}

//-------------------------------------------------------------------- operators
vec2& vec2::operator = (const vec2& source){

    if (this != &source) {

        this->x = source.x;
        this->y = source.y;

    }

    return *this;
}

float vec2::operator  []  (int index) const{

    switch(index){

        case 0: return x;
        case 1: return y;
        default:
            throw std::out_of_range("vec2 index out of range");

    }

}

float& vec2::operator [] (int index){
    
    switch (index){

        case 0: return x;
        case 1: return y;
        default:
            throw std::out_of_range("vec2 index out of range");

    }
}

vec2 vec2::operator - () const{

    vec2 temp = *this;

    temp.x = -temp.x;
    temp.y = -temp.y;

    return temp;

}


vec2 vec2::operator * (float num) const{

    vec2 temp = *this;

    temp.x *= num;
    temp.y *= num;

    return temp;

}

vec2& vec2::operator *= (float num){

    this->x *= num;
    this->y *= num;

    return *this;

}

vec2 vec2::operator + (const vec2& other) const{

    vec2 temp = *this;

    temp.x += other.x;
    temp.y += other.y;

    return temp;

}

vec2 vec2::operator - (const vec2& other) const{

    vec2 temp = *this;

    temp.x -= other.x;
    temp.y -= other.y;

    return temp;

}

vec2 vec2::operator * (const vec2& other) const{

    vec2 temp = *this;

    temp.x *= other.x;
    temp.y *= other.y;

    return temp;

}

vec2& vec2::operator += (const vec2& other){

    this->x += other.x;
    this->y += other.y;

    return *this;

}

vec2& vec2::operator -= (const vec2& other){

    this->x -= other.x;
    this->y -= other.y;

    return *this;

}

vec2& vec2::operator *= (const vec2& other){

    this->x *= other.x;
    this->y *= other.y;

    return *this;

}

vec2& vec2::operator ++ (){

    this->x += 1.0f;
    this->y += 1.0f;

    return *this;

}

vec2 vec2::operator ++ (int){

    vec2 temp = *this;

    ++(*this);

    return temp;

}

vec2& vec2::operator -- (){

    this->x -= 1.0f;
    this->y -= 1.0f;

    return *this;

}

vec2 vec2::operator -- (int){

    vec2 temp = *this;

    --(*this);

    return temp;

}

bool vec2::operator == (const vec2& other) const{

    return this->x == other.x
        && this->y == other.y;

}

bool vec2::operator != (const vec2& other) const{

    return !(*this == other);

}

//---------------------------------------------------- utility functions
float vec2::length() const
{
    return std::sqrt(lengthSquared());
}

float vec2::lengthSquared() const
{
    return x * x
         + y * y;
}

void vec2::normalize()
{
    *this = normalized();
}

vec2 vec2::normalized() const
{
    float len = length();

    if (len == 0.0f)
        return vec2();

    return vec2(
        x / len,
        y / len
    );
}

vec2 vec2::normalized(const vec2& vector)
{
    return vector.normalized();
}

float vec2::dot(const vec2& other) const
{
    return x * other.x
         + y * other.y;
}

float vec2::dot(const vec2& lhs, const vec2& rhs)
{
    return lhs.dot(rhs);
}

//--------------------------------------------------------- non-member functions

vec2 operator * (float num, const vec2& obj)
{
    vec2 temp(obj);

    temp *= num;

    return temp;
}

std::ostream& operator << (std::ostream& out, const vec2& obj)
{
    out << "{"
        << obj[0]
        << ", "
        << obj[1]
        << "}";

    return out;
}
