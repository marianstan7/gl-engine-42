//
// Vector3.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Fri Feb 17 12:51:39 2012 loick michard
// Last update Thu Feb 23 14:45:42 2012 gael jochaud-du-plessix
//

#ifndef _GLE_VECTOR3_HPP_
# define _GLE_VECTOR3_HPP_

# include <iostream>
# include <cmath>

namespace gle {
  
  //! Class for manipulate 3D vectors.
  /*
    This class encapsulates all 3D vectors operations/
    \tparam T Type of 3D vctors values.
  */
  
  template <typename T>
  class Vector3
  {
  public:

    //! Default constructor.
    /*!
      Initialize a 3D vector:
      \f[ \left(\begin{array}{c}0\\
      0\\
      0\end{array}\right)
      \f]
     */
    
    Vector3() : x(0), y(0), z(0)
    {

    }
    
    //! Constructor for initialization.
    /*!
      Initialize a 3D vector:
      \f[ \left(\begin{array}{c}x\\
      y\\
      z\end{array}\right)
      \f]
      \param x x component
      \param y y component
      \param z z component
    */
    
    Vector3(T const& x, T const& y, T const& z) : x(x), y(y), z(z)
    {

    }

    //! Add a vector
    /*!
      \param other 3D vector to add
    */
    Vector3& operator+=(Vector3 const& other)
    {
      x += other.x;
      y += other.y;
      z += other.z;
      return (*this);
    }

    //! Multiply a vector
    /*!
      \param value multiply vector by this value
    */

    Vector3& operator*=(T const& value)
    {
      x *= value;
      y *= value;
      z *= value;
      return (*this);
    }

    //! Normalize the vector
    void normalize()
    {
      T len = sqrt(x * x + y * y + z * z);

      if (len == 0)
	return;
      len = 1.0 / len;
      x *= len;
      y *= len;
      z *= len;
    }

    //! x component of 3D vector
    T x;
    //! y component of 3D vector
    T y;
    //! z component of 3D vector
    T z;
  };
}

template <typename T>
T operator*(gle::Vector3<T> const & v1, gle::Vector3<T> const & v2)
{
  return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

template <typename T>
gle::Vector3<T> operator^(gle::Vector3<T> const & v1,
			  gle::Vector3<T> const & v2)
{
  return (gle::Vector3<T>(
			  v1.y * v2.z - v1.z * v2.y,
			  v1.z * v2.x - v1.x * v2.z,
			  v1.x * v2.y - v1.y * v2.x
			  ));
}

template <typename T>
gle::Vector3<T> operator-(gle::Vector3<T> const & v1,
			  gle::Vector3<T> const & v2)
{
  return (gle::Vector3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
}

#endif // _VECTOR3_HPP_
