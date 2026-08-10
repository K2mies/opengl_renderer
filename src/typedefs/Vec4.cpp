#include  "Vec4.h"

//----------------------------------------------------------------- constructors
vec4::vec4()  {

	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 1.0f;

}

vec4::vec4( float num1, float num2, float num3, float num4 )  {

	this->x = num1;
	this->y = num2;
	this->z = num3;
	this->w = num4;

}

vec4::vec4( float num1, float num2, float num3 )  {

  this->x = num1;
  this->y = num2;
  this->z = num3;
  this->w = 1.0f;

}

vec4::vec4( const vec3& axis, float w)  {

  this->x = axis.x;
  this->y = axis.y;
  this->z = axis.z;
  this->w = w;

}

vec4::vec4( const vec4& source ){ *this = source; }

//-------------------------------------------------------------------- operators
vec4& vec4::operator = ( const vec4& source ) {

	if ( this != &source )
	{
		this->x = source.x;
		this->y = source.y;
		this->z = source.z;
		this->w = source.w;
	}
	return ( *this );

}

float vec4::operator [] ( int index ) const {

  switch (index)  {

          case 0: return x;
          case 1: return y;
          case 2: return z;
          case 3: return w;
          default: throw std::out_of_range("vec4 index out of range");

      }

}

float&  vec4::operator [] ( int index ) {

  switch (  index )  {

          case 0: return x;
          case 1: return y;
          case 2: return z;
          case 3: return w;
          default: throw std::out_of_range("vec4 index out of range");

      }

}

vec4  vec4::operator - () const {

  vec4	temp = *this;
  temp[0] = -temp[0];
  temp[1] = -temp[1];
  temp[2] = -temp[2];
  temp[3] = -temp[3];
  return ( temp );

}

vec4  vec4::operator * ( float num ) const {

	vec4	temp = *this;
	temp.x *= num;
	temp.y *= num;
	temp.z *= num;
	temp.w *= num;
	return ( temp );

}

vec4& vec4::operator *= ( float num ) {

	this->x *= num;
	this->y *= num;
	this->z *= num;
	this->w *= num;
	return ( *this );

}

vec4  vec4::operator + ( const vec4& other )  const  {

	vec4	temp = *this;
	temp.x += other.x;
	temp.y += other.y;
	temp.z += other.z;
	temp.w += other.w;
	return ( temp ); 

}

vec4  vec4::operator - ( const vec4& other )  const {

  vec4	temp = *this;
  temp.x -= other.x;
  temp.y -= other.y;
  temp.z -= other.z;
  temp.w -= other.w;
  return ( temp );

}

vec4  vec4::operator * ( const vec4& other ) const {

	vec4	temp = *this;
	temp.x *= other.x;
	temp.y *= other.y;
	temp.z *= other.z;
	temp.w *= other.w;
	return ( temp );

}

vec4& vec4::operator += ( const vec4& other ) {

	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	this->w += other.w;
	return ( *this );

}

vec4& vec4::operator -= ( const vec4& other ) {

	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	this->w -= other.w;
	return ( *this );

}

vec4&	vec4::operator *= ( const vec4& other ) {

	this->x *= other.x;
	this->y *= other.y;
	this->z *= other.z;
	this->w *= other.w;

	return ( *this );

}

vec4& vec4::operator ++ ()  {

	this->x += 1;
	this->y += 1;
	this->z += 1;
	this->w += 1;
	return ( *this );

}

vec4  vec4::operator ++ ( int ) {

	vec4 temp = *this;
	++*this;
	return ( temp );

}

vec4& vec4::operator -- ()  {

	this->x -= 1;
	this->y -= 1;
	this->z -= 1;
	this->w -= 1;
	return ( *this );

}

vec4  vec4::operator -- ( int )  {

	vec4 temp = *this;
	--*this;
	return ( temp );

}

bool  vec4::operator == ( const vec4& other ) const {

	if ( ( this->x == other.x ) && ( this->y == other.y ) && ( this->z == other.z) && ( this->w == other.w) )
	       return ( true );

	return ( false );	

}

bool  vec4::operator != ( const vec4& other ) const
{ return ( !( *this == other ) ); }

vec4::~vec4(){}

//--------------------------------------------------------- non-member functions 
vec4  operator * ( int num, const vec4& obj )  {

	vec4	temp ( obj );
	temp *= num;
	return ( temp );

}

std::ostream&	operator	<<	( std::ostream& out, const vec4& obj )  {

  out         << "{"
    << obj[0] << ", "
    << obj[1] << ", "
    << obj[2] << ", "
    << obj[3]
    << "}";

	return ( out );

}
