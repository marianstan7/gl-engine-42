//
// RenderBuffer.hpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu May 31 17:40:59 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 17:14:59 2012 gael jochaud-du-plessix
//

#ifndef _RENDER_BUFFER_HPP_
# define _RENDER_BUFFER_HPP_

# include <gle/opengl.h>

namespace gle {

  //! %Buffer used to store rendered datas
  /*!
    Render buffer are used as attachment to frame buffer
    in order to store rendered data such as depth or stencil infos
   */

  class RenderBuffer {
  public:

    //! Types of render buffers
    /*!
      There are different types of render buffers, depending on the
      data they can receive
     */

    enum Type {      
      RGB = GL_RGB,
      /*! Type for storing RGB datas */
      RGBA = GL_RGBA,
      /*! Type for storing RGBA datas */
      Depth = GL_DEPTH_COMPONENT,
      /*! Type for storing Depth datas */
      Stencil = GL_STENCIL_INDEX
      /*! Type for storing Stencil datas */
    };

    //! Constructs a render buffer
    /*!
      Creates a new render buffer of the specified type.
      If width and height are specified and non zero, a storage is also created for the buffer.

      \param type Type of the render buffer
      \param width Width of the buffer
      \param height Height of the buffer
     */

    RenderBuffer(Type type, GLuint width=0, GLuint height=0);

    //! Destruct a render buffer

    ~RenderBuffer();

    //! Bind the render buffer for current operations

    void	bind() const;    

    //! Set the storage of the render buffer
    /*!
      Set the storage user by the render buffer for its datas

      \param width Width of the buffer
      \param height Height of the buffer
     */

    void	setStorage(GLuint width, GLuint height);

    //! Returns the OpenGL ID of the render buffer

    GLuint	getId() const;

    //! Returns the width of the render buffer

    GLuint	getWidth() const;

    //! Returns the height of the render buffer

    GLuint	getHeight() const;

  private:
    GLuint	_id;
    Type	_type;
    GLuint	_width;
    GLuint	_height;
  };

}

#endif
