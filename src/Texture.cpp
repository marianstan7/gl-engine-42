//
// Texture.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 29 19:37:40 2012 gael jochaud-du-plessix
// Last update Thu Jun  7 14:12:04 2012 gael jochaud-du-plessix
//

#include <Texture.hpp>
#include <Exception.hpp>

gle::Texture::Texture(std::string filename, Type type, InternalFormat internalFormat) :
  _id(0), _type(type), _internalFormat(internalFormat), _width(0), _height(0),
  _useMipmap(true)
{
  glGenTextures(1, &_id);
  setData(filename);
  setUseMipmap(_useMipmap);
}

gle::Texture::Texture(GLuint width, GLuint height, Type type, InternalFormat internalFormat) :
  _id(0), _type(type), _internalFormat(internalFormat), _width(width), _height(height),
  _useMipmap(true)
{
  glGenTextures(1, &_id);
  if (width != 0 && height != 0)
    setData(NULL, width, height);
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

void gle::Texture::setData(std::string filename)
{
  sf::Image image;
  image.loadFromFile(filename);
  _width = image.getSize().x;
  _height = image.getSize().y;
  setData(image);
}

void gle::Texture::setData(sf::Image const &image)
{
  const sf::Uint8* pixelsPtr = image.getPixelsPtr();
  if (pixelsPtr == NULL)
    throw new gle::Exception::InvalidValue("Invalid texture image");
  setData((const char*)pixelsPtr);
}

void gle::Texture::setData(const char* pixelsPtr, GLuint width, GLuint height)
{
  if (width != 0 && height != 0)
    {
      _width = width;
      _height = height;
    }
  bind();
  glTexImage2D(GL_TEXTURE_2D, // Texture type
	       0, // Level of detail (0 = max)
	       _internalFormat, // Internal format
	       _width, // Width
	       _height, // Height
	       0, // This value must be 0
	       GL_RGBA, // Format of the pixel datas
	       GL_UNSIGNED_BYTE, // Data type of the pixel datas
	       pixelsPtr);
  gle::Exception::CheckOpenGLError("Texture::setData()");
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
