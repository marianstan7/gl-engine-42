//
// FrameBuffer.hpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu May 31 17:20:05 2012 gael jochaud-du-plessix
// Last update Mon Jun  4 15:17:08 2012 gael jochaud-du-plessix
//

#ifndef _GLE_FRAME_BUFFER_HPP_
# define _GLE_FRAME_BUFFER_HPP_

# include <gle/opengl.h>
# include <Texture.hpp>
# include <RenderBuffer.hpp>

namespace gle {

  class FrameBuffer {
  public:
    enum Attachment {
      AttachmentColor = GL_COLOR_ATTACHMENT0,
      AttachmentDepth = GL_DEPTH_ATTACHMENT,
      AttachmentStencil = GL_STENCIL_ATTACHMENT
    };

    static FrameBuffer& getDefaultFrameBuffer();

    FrameBuffer(GLint id=-1);
    virtual ~FrameBuffer();

    virtual void update();

    void bind() const;

    void attach(gle::Texture const& texture, Attachment attachment);
    void detach(gle::Texture const& texture, Attachment attachment);

    void attach(gle::RenderBuffer const& renderBuffer, Attachment attachment);
    void detach(gle::RenderBuffer const& renderBuffer, Attachment attachment);

    GLenum	getStatus() const;
    bool	isComplete() const;
    
    GLuint	getId() const;

  private:
    GLuint	_id;
  };

}

#endif
