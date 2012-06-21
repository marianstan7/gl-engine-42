//
// Texture.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 29 19:37:40 2012 gael jochaud-du-plessix
// Last update Thu Jun 21 21:12:09 2012 gael jochaud-du-plessix
//

#include <Texture.hpp>
#include <Exception.hpp>
#include <iostream>

gle::Texture::Texture(const Image& image, Type type, InternalFormat internalFormat) :
  _id(0), _type(type), _internalFormat(internalFormat), _width(0), _height(0),
  _useMipmap(true)
{
  glGenTextures(1, &_id);
  setData(image);
  setUseMipmap(_useMipmap);
}

gle::Texture::Texture(GLuint width, GLuint height, Type type, InternalFormat internalFormat) :
  _id(0), _type(type), _internalFormat(internalFormat), _width(width), _height(height),
  _useMipmap(true)
{
  glGenTextures(1, &_id);
  if (width != 0 && height != 0)
    setData((const char*)NULL, (GLuint)width, (GLuint)height);
  setUseMipmap(_useMipmap);
}

gle::Texture::~Texture()
{
  glDeleteTextures(1, &_id);
}

void gle::Texture::bind()
{
  glBindTexture(_type, _id);
}

void gle::Texture::unbind()
{
  glBindTexture(_type, 0);
}

void gle::Texture::setData(const Image& image, Target target, bool bindTexture)
{
  setData((const char*)image.getData(), image.getWidth(), image.getHeight(), target, bindTexture);
}

void gle::Texture::setData(const char* data, GLuint width, GLuint height, Target target, bool bindTexture)
{
  if (bindTexture)
    bind();
  if (width && height)
    {
      _width = width;
      _height = height;   
    }
  glTexImage2D(target, // Texture type
	       0, // Level of detail (0 = max)
	       _internalFormat, // Internal format
	       _width, // Width
	       _height, // Height
	       0, // This value must be 0
	       GL_RGBA, // Format of the pixel datas
	       GL_UNSIGNED_BYTE, // Data type of the pixel datas
	       data);
  generateMipmap();
  if (bindTexture)
    unbind();
}

void gle::Texture::generateMipmap()
{
  if (!_useMipmap)
    return ;
  bind();
  glGenerateMipmap(_type);  
}

GLuint	gle::Texture::getId() const
{
  return (_id);
}

gle::Texture::Type	gle::Texture::getType() const
{
  return (_type);
}

void	gle::Texture::setUseMipmap(bool use)
{
  _useMipmap = use;
  bind();
  if (_useMipmap)
    {
      glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      glGenerateMipmap(_type);
    }
  else
    glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

gle::Rectf	gle::Texture::getSize() const
{
  return (Rectf(0, 0, _width, _height));
}
