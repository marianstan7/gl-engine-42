//
// RenderBuffer.cpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu May 31 17:42:09 2012 gael jochaud-du-plessix
// Last update Thu May 31 18:07:02 2012 gael jochaud-du-plessix
//

#include "RenderBuffer.hpp"

#include "Exception.hpp"

gle::RenderBuffer::RenderBuffer(Type type, GLuint width, GLuint height) :
  _id(0), _type(type), _width(width), _height(height)
{
  glGenRenderbuffers(1, &_id);
  if (_width > 0 && _height > 0)
    setStorage(_width, _height);
}

gle::RenderBuffer::~RenderBuffer()
{
  glDeleteRenderbuffers(1, &_id);
}


void gle::RenderBuffer::bind() const
{
  glBindRenderbuffer(GL_RENDERBUFFER, _id);
}

void	gle::RenderBuffer::setStorage(GLuint width, GLuint height)
{
  bind();
  _width = width;
  _height = height;
  glRenderbufferStorage(GL_RENDERBUFFER, _type, _width, _height);
  gle::Exception::CheckOpenGLError("glRenderbufferStorage()");
}

GLuint	gle::RenderBuffer::getId() const
{
  return (_id);
}

GLuint	gle::RenderBuffer::getWidth() const
{
  return (_width);
}

GLuint	gle::RenderBuffer::getHeight() const
{
  return (_height);
}
