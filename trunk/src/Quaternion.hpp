//
// Quaternion.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Wed Feb 15 18:22:27 2012 loick michard
// Last update Wed Feb 22 21:46:37 2012 gael jochaud-du-plessix
//

#ifndef _GLE_QUATERNION_HPP
# define _GLE_QUATERNION_HPP

# include <iostream>
# include <cmath>
# include <Matrix4.hpp>

namespace gle {
  
  //! Class for manipulate quaternions.
  /*!
    This class encapsulates all quaternions operations.
    \tparam T Type of quaternions values.
  */

  template <typename T>
  class Quaternion
  {
  public:

    //! Default constructor
    /*!
      Build a quaternion : 1 + 0i + 0j + 0k
    */

    Quaternion() : w(1), x(0), y(0), z(0)
    {
      
    }

    //! Constructor with parameters
    /*!
      Build a quaternion : w + 0x + 0y + 0z
      \param w Parameter w
      \param x Parameter x
      \param y Parameter y
      \param z Parameter z
    */

    Quaternion(T const& w, T const& x, T const& y, T const& z) :
      w(w), x(x), y(y), z(z)
    {
      
    }

    //! Default destructor

    ~Quaternion()
    {

    }
    
    //! Rotates a quaternion
    /*
      Apply a rotation of angle in degrees around an axis.
      \param xa X component of axis
      \param ya Y component of axis
      \param za Z component of axis
      \param angle Angle of rotation in degrees
    */

    void setRotation(T const& xa, T const& ya, T const& za, T const& angle)
    {
      float sinus = sin(angle * M_PI / 360.0);
      T tx, ty, tz, len;
      len = sqrt(xa * xa + ya * ya + za * za);
      if (len)
	{
	  len = 1.0 / len;
	  tx = xa * len;
	  ty = ya * len;
	  tz = za * len;
	}
      else
	tx = ty = tz = 0;
      x = (tx * sinus);
      y = (ty * sinus);
      z = (tz * sinus);
      w = cos(angle * M_PI / 360.0);
    }

    //! Rotates a quaternion
    /*
      Apply a rotation of angle in degrees around an axis.
      \param axis Axis vector
      \param angle Angle of rotation in degrees
    */

    void setRotation(Vector3<T> axis, T const& angle)
    {
      return (setRotation(axis.x, axis.y, axis.z, angle));
    }

    //! Get matrix from quaternion

    Matrix4<T> getMatrix()
    {
      return (Matrix4<T>(1 - 2 * y * y - 2 * z * z,
			 2 * x * y  - 2 * z * w,
			 2 * x * y  + 2 * y * w, 0.0,
			 2 * x * y + 2 * z * w,
			 1 - 2 * x * x - 2 * z * z,
			 2 * y * z  - 2 * x * w, 0.0,
			 2 * x * z  - 2 * y * w,
			 2 * y * z + 2 * x * w,
			 1 - 2 * x * x  - 2 * y * y, 0.0,
			 0.0, 0.0, 0.0, 1.0));
    }

    //! w component in w + xi + yj + zk
    T w;
    //! x component in w + xi + yj + zk
    T x;
    //! y component in w + xi + yj + zk
    T y;
    //! z component in w + xi + yj + zk
    T z;
  };
}

template <typename T>
std::ostream& operator<<(std::ostream& os, gle::Quaternion<T> const &q)
{
  os << "Quaternion : " << q.w << " + " << q.x << "i + " <<
    q.y << "j + " << q.z << "k";
  return (os);
}

#endif // _GLE_QUATERNION_HPP
