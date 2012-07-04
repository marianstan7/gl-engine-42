//
// Image.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Jun  7 23:22:21 2012 loick michard
// Last update Wed Jul  4 15:24:34 2012 loick michard
//

#ifndef _IMAGE_HPP_
# define _IMAGE_HPP_

# include <SFML/Graphics/Image.hpp>
# include <string>
# include <gle/opengl.h>
# include <iostream>

namespace gle
{
  //! Class to load and store images on CPU
  class Image
  {
  public:
    //! Construct an empty image
    Image();

    //! Construct an image on CPU
    /*!
      Enable to construct an image from different ways:
	- const std::string& filename
	- sf::Image const & image
	- const char* pixelsPtr, GLuint width, GLuint height
     */
    template <class... Args>
    Image(const Args&... args) : _data(NULL), _width(0), _height(0)
    {
      this->setData(args...);
    }

    //! Destroy an image
    ~Image();
    
    //! Load image and store data from a filename
    /*!
      \param filename Image path
     */
    void setData(const std::string& filename);

    //! Store data from a sfml image format
    /*!
      \param image A SFML image
     */
    void setData(sf::Image const & image);

    //! Store data from a pixels buffer
    /*!
      \param pixelsPtr Pixels buffer of size width x height
      \param width Image width
      \param height Image height
     */
    void setData(const char* pixelsPtr, GLuint width, GLuint height);

    //! Return image pixels buffer
    char* getData() const;

    //! Return image width
    GLuint getWidth() const;

    //! Return image height
    GLuint getHeight() const;
    
  private:
    char*		_data;
    GLuint              _width;
    GLuint              _height;
  };
};

#endif
