//
// TextureFrameBuffer.cpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Jun  4 14:52:23 2012 gael jochaud-du-plessix
// Last update Mon Jun  4 15:13:42 2012 gael jochaud-du-plessix
//

#include "TextureFrameBuffer.hpp"

gle::TextureFrameBuffer::TextureFrameBuffer(GLuint width, GLuint height) :
  FrameBuffer(), _renderTexture(), _depthRenderBuffer()
{
  _renderTexture = new gle::Texture(width, height, gle::Texture::Texture2D, gle::Texture::RGB);
  _depthRenderBuffer = new gle::RenderBuffer(gle::RenderBuffer::Depth, width, height);
  attach(*_renderTexture, FrameBuffer::AttachmentColor);
  attach(*_depthRenderBuffer, FrameBuffer::AttachmentDepth);
}

gle::TextureFrameBuffer::~TextureFrameBuffer()
{
  delete _renderTexture;
  delete _depthRenderBuffer;
}

void gle::TextureFrameBuffer::update()
{
  _renderTexture->generateMipmap();
}

gle::Texture*            gle::TextureFrameBuffer::getRenderTexture() const
{
  return (_renderTexture);
}

gle::RenderBuffer*       gle::TextureFrameBuffer::getDepthRenderBuffer() const
{
  return (_depthRenderBuffer);
}

