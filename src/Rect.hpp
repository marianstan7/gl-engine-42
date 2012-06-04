//
// Rect.hpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Jun  4 15:49:17 2012 gael jochaud-du-plessix
// Last update Mon Jun  4 16:03:08 2012 gael jochaud-du-plessix
//

#ifndef _RECT_HPP_
# define _RECT_HPP_

# include <gle/opengl.h>

namespace gle {

  template <class T>
  struct Rect {
    Rect(T x, T y, T width, T height) :
      x(x), y(y), width(width), height(height)
    {
    }

    T	x;
    T	y;
    T	width;
    T	height;
  };

  typedef Rect<GLint> Recti;
  typedef Rect<GLuint> Rectui;
  typedef Rect<GLfloat> Rectf;
}

#endif
