//
// Rect.hpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Jun  4 15:49:17 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 16:57:45 2012 gael jochaud-du-plessix
//

#ifndef _RECT_HPP_
# define _RECT_HPP_

# include <gle/opengl.h>

namespace gle {

  //! Representation of simple 2D rectangles

  template <class T>
  struct Rect {

    //! Constructs a rectangle
    /*!
      \param x X position of the rectangle
      \param y Y position of the rectangle
      \param width Width of the rectangle
      \param height Height of the rectangle
     */

    Rect(T x, T y, T width, T height) :
      x(x), y(y), width(width), height(height)
    {
    }

    //! X coord of the rectangle

    T	x;

    //! Y coord of the rectangle
    T	y;

    //! Width of the rectangle

    T	width;

    //! Height of the rectangle

    T	height;
  };

  //! Rectangle with integer values
  typedef Rect<GLint> Recti;

  //! Rectangle with unsigned integer values
  typedef Rect<GLuint> Rectui;

  //! Rectangle with floating values
  typedef Rect<GLfloat> Rectf;
}

#endif
