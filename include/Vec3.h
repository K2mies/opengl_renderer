#ifndef VEC3_HPP
#define VEC3_HPP

#include<iostream>
#include<cmath>

class vec3
{

public:

  float x;
  float y;
  float z;

	//--------------------------------------------------------- constructors
	vec3    ();
	vec3    ( float num1, float num2, float num3 );
	vec3    ( const vec3& source );

	//------------------------------------------------------------ operators
	vec3&	  operator	=	  ( const vec3& source );

	float	  operator	[]	(       int   index )	  const;
	float&  operator	[]	(       int   index );

	vec3	  operator	-	  ()				              const;

	vec3	  operator	*   (       float num )			const;
	vec3&	  operator	*=	(       float num );

	vec3	  operator	+   ( const vec3& other )	  const;
	vec3	  operator	-	  ( const vec3& other )	  const;
	vec3	  operator	*   ( const vec3& other )	  const;

	vec3&	  operator	+=	( const vec3& other );
	vec3&	  operator	-=	( const vec3& other );
	vec3&	  operator	*=	( const vec3& other );

	vec3&	  operator	++	();
	vec3	  operator	++	(       int );
	vec3&	  operator	--	();
	vec3	  operator	--	(       int );

	bool	  operator	==	( const vec3& other )		const;
	bool	  operator	!=	( const vec3& other )		const;
  
  //---------------------------------------------------- utility functions

  float   length()                              const;
  
  float   lengthSquared()                       const;
  
  void    normalize();
  
  static  vec3       normalized              (  const vec3& vector  );
  vec3               normalized         ()      const;

  
  float   dot       ( const vec3& other )       const;
  
  vec3    cross     ( const vec3& other )       const;
  static  vec3      cross                      (const vec3& vector_a, const vec3& vector_b);


  //----------------------------------------------------------- destructor
	~vec3   () = default;
};

//--------------------------------------------------------- non-member functions 
vec3		      operator	*    ( float num, const vec3& obj );
std::ostream&	operator	<<	( std::ostream& out, const vec3& obj );

#endif
