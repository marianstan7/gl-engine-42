//
// Image.cpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Jun  7 23:27:45 2012 loick michard
// Last update Sun Jun 17 20:39:48 2012 loick michard
//

#include <Image.hpp>
#include <Exception.hpp>

gle::Image::Image() : _data(NULL), _width(0), _height(0)
{

}

gle::Image::~Image()
{
  delete [] _data;
}

void gle::Image::setData(const std::string& filename)
{
  sf::Image image;

  image.loadFromFile(filename);
  _width = image.getSize().x;
  _height = image.getSize().y;
  setData(image);
}

void gle::Image::setData(sf::Image const &image)
{
  const sf::Uint8* pixelsPtr = image.getPixelsPtr();

  if (pixelsPtr == NULL)
    throw new gle::Exception::InvalidValue("Invalid image");
  setData((const char*)pixelsPtr, image.getSize().x, image.getSize().y);
}

void gle::Image::setData(const char* pixelsPtr, GLuint width, GLuint height)
{
  if (width && height)
    {
      _width = width;
      _height = height;
    }
  if (_data)
    delete [] _data;
  _data = new char[width * height * 4];
  for (size_t i = 0; i < width * height * 4; ++i)
    _data[i] = pixelsPtr[i];
}

char* gle::Image::getData() const
{
  return (_data);
}

GLuint gle::Image::getWidth() const
{
  return (_width);
}

GLuint gle::Image::getHeight() const
{
  return (_height);
}
