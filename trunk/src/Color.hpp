//
// Color.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Fri Feb 17 12:51:39 2012 loick michard
// Last update Fri Apr 13 16:54:42 2012 gael jochaud-du-plessix
//

#ifndef _GLE_COLOR_HPP_
# define _GLE_COLOR_HPP_

# include <iostream>
# include <cmath>

namespace gle {
  
  //! Class for manipulate RGBA colors.
  /*
    This class encapsulates all RGBA colors operations
    \tparam T Type of RGBA values.
  */
  
  template <typename T>
  class Color
  {
  public:

    //! Default constructor.
    /*!
      Initialize a color
    */
    
    Color() : r(0), g(0), b(0), a(0)
    {

    }
    
    //! Constructor for initialibation.
    /*!
      Initialibe a RGBA color
      \param r r component
      \param g g component
      \param b b component
      \param a a component
    */
    
    Color(T const& r, T const& g, T const& b, T const& a=1.0) : r(r), g(g), b(b), a(a)
    {

    }

    //! Add a color
    /*!
      \param other RGBA color to add
    */
    Color& operator+=(Color const& other)
    {
      r += other.r;
      g += other.g;
      b += other.b;
      a += other.a;
      return (*this);
    }

    //! Multiply a color
    /*!
      \param value multiply color by this value
    */

    Color& operator*=(T const& value)
    {
      r *= value;
      g *= value;
      b *= value;
      a *= value;
      return (*this);
    }

    //! r component of the RGBA Color
    T r;
    //! g component of the RGBA Color
    T g;
    //! b component of the RGBA Color
    T b;
    //! a component of the RGBA Color
    T a;
  };

  typedef Color<GLfloat> fColor;
}

#endif // _COLOR_HPP_
