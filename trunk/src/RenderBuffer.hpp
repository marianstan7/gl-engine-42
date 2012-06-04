//
// RenderBuffer.hpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu May 31 17:40:59 2012 gael jochaud-du-plessix
// Last update Thu May 31 17:51:57 2012 gael jochaud-du-plessix
//

#ifndef _RENDER_BUFFER_HPP_
# define _RENDER_BUFFER_HPP_

# include <gle/opengl.h>

namespace gle {

  class RenderBuffer {
  public:

    enum Type {
      RGB = GL_RGB,
      RGBA = GL_RGBA,
      Depth = GL_DEPTH_COMPONENT,
      Stencil = GL_STENCIL_INDEX
    };

    RenderBuffer(Type type, GLuint width=0, GLuint height=0);
    ~RenderBuffer();

    void	bind() const;
    void	setStorage(GLuint width, GLuint height);

    GLuint	getId() const;
    GLuint	getWidth() const;
    GLuint	getHeight() const;

  private:
    GLuint	_id;
    Type	_type;
    GLuint	_width;
    GLuint	_height;
  };

}

#endif
