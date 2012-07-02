//
// FrameBuffer.cpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu May 31 17:21:30 2012 gael jochaud-du-plessix
// Last update Tue Jun 26 15:37:22 2012 gael jochaud-du-plessix
//

#include "FrameBuffer.hpp"
#include "Exception.hpp"

gle::FrameBuffer& gle::FrameBuffer::getDefaultFrameBuffer()
{
  static gle::FrameBuffer defaultFB(0);
  return (defaultFB);
}

gle::FrameBuffer::FrameBuffer(GLint id) :
  _id(0)
{
  if (id == -1)
    glGenFramebuffers(1, &_id);
  else
    _id = id;
}

gle::FrameBuffer::~FrameBuffer()
{
  if (_id != 0)
    glDeleteFramebuffers(1, &_id);
}

void gle::FrameBuffer::update()
{
}

void gle::FrameBuffer::bind() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void gle::FrameBuffer::attach(gle::Texture const& texture, Attachment attachment)
{
  bind();
  if (texture.getType() == gle::Texture::Texture2D)
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment,
			   GL_TEXTURE_2D,
			   texture.getId(),
			   0);
  gle::Exception::CheckOpenGLError("Attach texture");
}

void gle::FrameBuffer::detach(gle::Texture const& texture, Attachment attachment)
{
  bind();
  if (texture.getType() == gle::Texture::Texture2D)
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment,
			   GL_TEXTURE_2D,
			   0,
			   0);
  gle::Exception::CheckOpenGLError("Detach texture");
}

void gle::FrameBuffer::attach(gle::RenderBuffer const& renderBuffer, Attachment attachment)
{
  bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment,
			    GL_RENDERBUFFER,
			    renderBuffer.getId());
  gle::Exception::CheckOpenGLError("Attach render buffer");
}

void gle::FrameBuffer::detach(gle::RenderBuffer const& renderBuffer, Attachment attachment)
{
  (void)renderBuffer;
  bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment,
			    GL_RENDERBUFFER,
			    0);
  gle::Exception::CheckOpenGLError("Detach render buffer");
}

GLenum	gle::FrameBuffer::getStatus() const
{
  bind();
  return (glCheckFramebufferStatus(GL_FRAMEBUFFER));
}

bool	gle::FrameBuffer::isComplete() const
{
  return (getStatus() == GL_FRAMEBUFFER_COMPLETE);
}

GLuint  gle::FrameBuffer::getId() const
{
  return (_id);
}
