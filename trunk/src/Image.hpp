//
// Image.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Jun  7 23:22:21 2012 loick michard
// Last update Sat Jun  9 12:01:08 2012 loick michard
//

#ifndef _IMAGE_HPP_
# define _IMAGE_HPP_

# include <SFML/Graphics/Image.hpp>
# include <string>
# include <gle/opengl.h>
# include <iostream>

namespace gle
{
  class Image
  {
  public:
    Image();
    template <class... Args>
    Image(const Args&... args) : _data(NULL), _width(0), _height(0)
    {
      this->setData(args...);
    }

    ~Image();
    
    void setData(const std::string& filename);
    void setData(sf::Image const & image);
    void setData(const char* pixelsPtr, GLuint width, GLuint height);

    char* getData() const;
    GLuint getWidth() const;
    GLuint getHeight() const;
    
  private:
    char*		_data;
    GLuint              _width;
    GLuint              _height;
  };
};

#endif
