//
// TextureFrameBuffer.hpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Jun  4 14:50:07 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 17:32:34 2012 gael jochaud-du-plessix
//

#ifndef _GLE_TEXTURE_FRAME_BUFFER_HPP_
# define _GLE_TEXTURE_FRAME_BUFFER_HPP_

# include <gle/opengl.h>
# include <FrameBuffer.hpp>

namespace gle {

  //! A simplified framebuffer for rendering to texture
  /*!
    Texture frame buffers automatically hold a color texture
    and a depth render buffer in order to be easily used for rendering
   */

  class TextureFrameBuffer : public FrameBuffer {
  public:

    //! Creates a new frame buffer with a specified width and height

    TextureFrameBuffer(GLuint width, GLuint height);    

    //! Destructs the frame buffer

    virtual ~TextureFrameBuffer();

    //! Update the frame buffer

    virtual void update();

    //! Returns the render texture
    /*!
      The render texture holds the result of rendering operations
     */

    Texture*		getRenderTexture() const;

    //! Returns the depth render buffer
    /*!
      The render buffer holds depth datas of rendering operations
     */

    RenderBuffer*	getDepthRenderBuffer() const;

  private:
    Texture*		_renderTexture;
    RenderBuffer*	_depthRenderBuffer;
  };

}

#endif
