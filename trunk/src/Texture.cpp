//
// Texture.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 29 19:37:40 2012 gael jochaud-du-plessix
// Last update Thu Apr 12 19:36:10 2012 loick michard
//

#include <Texture.hpp>
#include <Exception.hpp>

gle::Texture::Texture(std::string filename, Type type) :
  _id(0), _type(type)
{
  glGenTextures(1, &_id);
  setData(filename);
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
  setData(image);
}

void gle::Texture::setData(sf::Image const &image)
{
  const sf::Uint8* pixelsPtr = image.getPixelsPtr();
  if (pixelsPtr == NULL)
    throw new gle::Exception::InvalidValue("Invalid texture image");
  bind();
  glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, // Texture type
	       0, // Level of detail (0 = max)
	       GL_RGBA, // Internal format
	       image.getSize().x, // Width
	       image.getSize().y, // Height
	       0, // This value must be 0
	       GL_RGBA, // Format of the pixel datas
	       GL_UNSIGNED_BYTE, // Data type of the pixel datas
	       pixelsPtr);
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
    throw new gle::Exception::OpenGLError();
}
