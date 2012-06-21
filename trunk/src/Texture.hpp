//
// Texture.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 29 19:37:51 2012 gael jochaud-du-plessix
// Last update Sun Jun 17 20:48:28 2012 loick michard
//

#ifndef _GLE_TEXTURE_HPP_
# define _GLE_TEXTURE_HPP_

# include <SFML/Graphics/Image.hpp>
# include <string>
# include <gle/opengl.h>
# include <Rect.hpp>
# include <Image.hpp>

namespace gle {

  class Texture {
  public:
    enum Type {
      Texture2D = GL_TEXTURE_2D,
      CubeMap = GL_TEXTURE_CUBE_MAP
    };
    
    enum Target {
      Tex2D = GL_TEXTURE_2D,
      PosX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
      NegX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
      PosY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
      NegY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
      PosZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
      NegZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    enum InternalFormat {
      RGBA = GL_RGBA,
      RGB = GL_RGB,
      CompressedRGBA = GL_COMPRESSED_RGBA
    };

    Texture(const Image& image, Type type=Texture2D, InternalFormat internalFormat=CompressedRGBA);
    Texture(GLuint width=0, GLuint height=0, Type type=Texture2D, InternalFormat internalFormat=CompressedRGBA);

    ~Texture();

    void bind();
    
    void setData(const Image& image, Target target=Tex2D);
    void setData(const char* data, GLuint width, GLuint height, Target target=Tex2D);

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
