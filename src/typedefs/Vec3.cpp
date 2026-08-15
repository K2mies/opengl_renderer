#include  "Vec3.h"

//----------------------------------------------------------------- constructors
vec3::vec3(){

	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;

}

vec3::vec3( float num1, float num2, float num3){

	this->x = num1;
	this->y = num2;
	this->z = num3;

}

vec3::vec3( const vec3& source ){ *this = source; }

//-------------------------------------------------------------------- operators
vec3& vec3::operator = ( const vec3& source ){

	if ( this != &source )
	{
		this->x = source.x;
		this->y = source.y;
		this->z = source.z;
  }
	  return ( *this );

}

float vec3::operator [] ( int index ) const{

  switch (index)
      {
          case 0: return x;
          case 1: return y;
          case 2: return z;
          default: throw std::out_of_range("vec3 index out of range");
      }

}

float& vec3::operator [] ( int index ){

  switch (index)
      {
          case 0: return x;
          case 1: return y;
          case 2: return z;
          default: throw std::out_of_range("vec3 index out of range");
      }

}

vec3 vec3::operator - () const{
  vec3	temp = *this;
  temp[0] = -temp[0];
  temp[1] = -temp[1];
  temp[2] = -temp[2];
  return ( temp );

}

vec3 vec3::operator * ( float num ) const{

	vec3	temp = *this;
	temp.x *= num;
	temp.y *= num;
	temp.z *= num;
	return ( temp );

}

vec3& vec3::operator *= ( float num ){

	this->x *= num;
	this->y *= num;
	this->z *= num;
	return ( *this );

}

vec3 vec3::operator + ( const vec3& other ) const{

	vec3	temp = *this;
	temp.x += other.x;
	temp.y += other.y;
	temp.z += other.z;
	return ( temp );

}

vec3 vec3::operator - ( const vec3& other ) const{

	vec3	temp = *this;
	temp.x -= other.x;
	temp.y -= other.y;
	temp.z -= other.z;
	return ( temp );

}

vec3 vec3::operator * ( const vec3& other ) const{

	vec3	temp = *this;
	temp.x *= other.x;
	temp.y *= other.y;
	temp.z *= other.z;
	return ( temp );

}

vec3& vec3::operator += ( const vec3& other ){

	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return ( *this );

}

vec3& vec3::operator -= ( const vec3& other ){

	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return ( *this );

}

vec3& vec3::operator *= ( const vec3& other ){

	this->x *= other.x;
	this->y *= other.y;
	this->z *= other.z;

	return ( *this );

}

vec3& vec3::operator ++ (){

	this->x += 1;
	this->y += 1;
	this->z += 1;
	return ( *this );

}

vec3 vec3::operator ++ ( int ){

	vec3 temp = *this;
	++*this;
	return ( temp );

}

vec3& vec3::operator -- (){

	this->x -= 1;
	this->y -= 1;
	this->z -= 1;
	return ( *this );

}

vec3 vec3::operator -- ( int ){

	vec3 temp = *this;
	--*this;
	return ( temp );

}

bool vec3::operator == ( const vec3& other ) const{

	if ( ( this->x == other.x ) && ( this->y == other.y ) && ( this->z == other.z) )
	       return ( true );
	return ( false );	
}

bool vec3::operator != ( const vec3& other ) const
{ return ( !( *this == other ) ); }

//---------------------------------------------------- utility functions

float vec3::length()  const{

   return std::sqrt(lengthSquared() );

}

float vec3::lengthSquared() const{

   return  x * x
         + y * y
         + z * z;

}

void  vec3::normalize(){
  
    *this = normalized();

}

vec3 vec3::normalized(const vec3& vector)
{
  return vector.normalized();
}

vec3 vec3::normalized() const{
   
  float len = length();

    if (len == 0.0f)
        return vec3(0.0f, 0.0f, 0.0f);

    return vec3(
        x / len,
        y / len,
        z / len
    );
}

float vec3::dot   ( const vec3& other ) const{
  
  return   x * other.x
         + y * other.y
         + z * other.z;

}

vec3  vec3::cross ( const vec3& other ) const{

  return vec3(

        y * other.z
      - z * other.y,

        z * other.x
      - x * other.z,

        x * other.y
      - y * other.x
    );

}

vec3 vec3::cross (const vec3& vector_a, const vec3& vector_b){

  return vector_a.cross(vector_b);

}


//--------------------------------------------------------- non-member functions 
vec3 operator * ( float num, const vec3& obj ){

	vec3	temp ( obj );
	temp *= num;
	return ( temp );

}

std::ostream& operator << ( std::ostream& out, const vec3& obj ){

    out         << "{"
    << obj[0] << ", "
    << obj[1] << ", "
    << obj[2]
    << "}";

	return ( out );

}
