#ifndef VEC4_HPP
#define VEC4_HPP

#include<iostream>

#include "Typedefs/Vec3.h"

class vec4
{

public:

  float x;
  float y;
  float z;
  float w;

	//--------------------------------------------------------- constructors
	vec4    ();
	vec4    ( float num1, float num2, float num3, float num4 );
	vec4    ( float num1, float num2, float num3 );

  vec4    ( const float axis[4] );
  vec4    ( const float axis[3], float w);

  vec4    ( const vec3& axis, float w);

	vec4    ( const vec4& source );

	//--------------------------------------------- static type constructors
  static vec4 position  (float x, float y, float z);
  static vec4 direction (float x, float y, float z);

  static vec4 position  (const vec3& vector);
  static vec4 direction (const vec3& vector);

  static vec4 position  (const float vector[3]);
  static vec4 direction (const float vector[3]);

	//------------------------------------------------------------ operators
	vec4&	  operator	=	  ( const vec4& source );

	float	  operator	[]	(       int   index )	  const;
	float&  operator	[]	(       int   index );

	vec4	  operator	-	  ()				              const;

	vec4	  operator	*   (       float num )			const;
	vec4&	  operator	*=	(       float num );

	vec4	  operator	+   ( const vec4& other )	  const;
	vec4	  operator	-   ( const vec4& other )	  const;
	vec4	  operator	*   ( const vec4& other )	  const;

	vec4&	  operator	+=	( const vec4& other );
	vec4&	  operator	-=	( const vec4& other );
	vec4&	  operator	*=	( const vec4& other );

	vec4&	  operator	++	();
	vec4	  operator	++	(       int );
	vec4&	  operator	--	();
	vec4	  operator	--	(       int );

	bool	  operator	==	( const vec4& other )		const;
	bool	  operator	!=	( const vec4& other )		const;

  //---------------------------------------------------- utility functions

  float   length()                              const;
  
  float   lengthSquared()                       const;
    
  void    normalize();
  
  static  vec4       normalized              (  const vec4& vector  );
  vec4               normalized         ()      const;
  
  float   dot       (  const vec4& other )             const;
  static  float dot (  const vec4& vector_a, 
                       const vec4& vector_b);

	~vec4   () = default;
};

//--------------------------------------------------------- non-member functions 
vec4		      operator	*  ( int num, const vec4& obj );
std::ostream&	operator	<<	( std::ostream& out, const vec4& obj );

#endif
