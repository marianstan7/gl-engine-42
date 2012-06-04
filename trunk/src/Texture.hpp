//
// Texture.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 29 19:37:51 2012 gael jochaud-du-plessix
// Last update Mon Jun  4 16:06:00 2012 gael jochaud-du-plessix
//

#ifndef _GLE_TEXTURE_HPP_
# define _GLE_TEXTURE_HPP_

# include <SFML/Graphics/Image.hpp>
# include <string>
# include <gle/opengl.h>
# include <Rect.hpp>

namespace gle {

  class Texture {
  public:
    enum Type {
      Texture2D = GL_TEXTURE_2D
    };

    enum InternalFormat {
      RGBA = GL_RGBA,
      RGB = GL_RGB,
      CompressedRGBA = GL_COMPRESSED_RGBA
    };

    Texture(std::string filename, Type type=Texture2D, InternalFormat internalFormat=CompressedRGBA);
    Texture(GLuint width=0, GLuint height=0, Type type=Texture2D, InternalFormat internalFormat=CompressedRGBA);

    ~Texture();

    void bind();
    
    void setData(std::string filename);
    void setData(sf::Image const & image);
    void setData(const char* pixelsPtr, GLuint width=0, GLuint height=0);

    void generateMipmap();

    GLuint	getId() const;
    Type	getType() const;
    void	setUseMipmap(bool use=true);
    Rectf	getSize() const;

  private:
    GLuint		_id;
    Type		_type;
    InternalFormat	_internalFormat;
    GLuint		_width;
    GLuint		_height;
    bool		_useMipmap;
  };

}

#endif /* _GLE_TEXTURE_HPP_ */
