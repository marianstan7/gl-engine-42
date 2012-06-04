//
// TextureFrameBuffer.hpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Jun  4 14:50:07 2012 gael jochaud-du-plessix
// Last update Mon Jun  4 15:13:11 2012 gael jochaud-du-plessix
//

#ifndef _GLE_TEXTURE_FRAME_BUFFER_HPP_
# define _GLE_TEXTURE_FRAME_BUFFER_HPP_

# include <gle/opengl.h>
# include <FrameBuffer.hpp>

namespace gle {

  class TextureFrameBuffer : public FrameBuffer {
  public:

    TextureFrameBuffer(GLuint width, GLuint height);
    virtual ~TextureFrameBuffer();

    virtual void update();

    Texture*		getRenderTexture() const;
    RenderBuffer*	getDepthRenderBuffer() const;

  private:
    Texture*		_renderTexture;
    RenderBuffer*	_depthRenderBuffer;
  };

}

#endif
