//
// Texture.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 29 19:37:51 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 16:22:38 2012 loick michard
//

#ifndef _GLE_TEXTURE_HPP_
# define _GLE_TEXTURE_HPP_

# include <SFML/Graphics/Image.hpp>
# include <string>
# include <gle/opengl.h>
# include <Rect.hpp>
# include <Image.hpp>

namespace gle {

  //! Handler of openGL texture
  class Texture {
  public:

    //! Type of the store openGL texture
    enum Type {
      Texture2D	= GL_TEXTURE_2D,
      CubeMap	= GL_TEXTURE_CUBE_MAP
    };
    
    //! Target for bind an openGL texture
    enum Target {
      Tex2D	= GL_TEXTURE_2D,
      PosX	= GL_TEXTURE_CUBE_MAP_POSITIVE_X,
      NegX	= GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
      PosY	= GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
      NegY	= GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
      PosZ	= GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
      NegZ	= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };


    //! GPU Internal storage format of texture
    enum InternalFormat {
      RGBA		= GL_RGBA,
      RGB		= GL_RGB,
      Depth		= GL_DEPTH_COMPONENT,
      CompressedRGBA	= GL_COMPRESSED_RGBA
    };

    //! OpenGL filter type for mipmap
    enum FilterType {
      Nearest	= GL_NEAREST,
      Linear	= GL_LINEAR
    };

    //! OpenGL wrap mode for mipmap
    enum WrapMode {
      Clamp	= GL_CLAMP_TO_EDGE
    };

    //! Create a texture from an existing CPU image
    /*!
      \param image CPU image
      \param type Type of the openGL texture to create
      \param internalFormat Internal format of the openGL texture to create
     */
    Texture(const Image& image, Type type=Texture2D, InternalFormat internalFormat=CompressedRGBA);

    //! Create an empty texture
    /*!
      \param width Texture width
      \param height Texture height
      \param type Type of the openGL texture to create
      \param internalFormat Internal format of the openGL texture to create
     */
    Texture(GLuint width=0, GLuint height=0, Type type=Texture2D, InternalFormat internalFormat=CompressedRGBA);

    //! Destroy texture
    ~Texture();

    //! Bind texture
    void bind();

    //! Unbind texture
    /*!
      Unbind texture by calling glBindTexture(textureId, 0)
     */
    void unbind();
    
    //! Set data from CPU image to GPU texture
    /*!
      \param image Image on CPU
      \param target Target of the openGL texture to create
      \param bindTexture If bindTexture equals true, this function auto bind and unbind the texture for the setData
     */
    void setData(const Image& image, Target target=Tex2D, bool bindTexture = true);

    //! Set data from CPU image to GPU texture
    /*!
      \param data Pixels buffer of the image
      \param width Image width
      \param height Image height
      \param target Target of the openGL texture to create
      \param bindTexture If bindTexture equals true, this function auto bind and unbind the texture for the setData
     */
    void setData(const char* data, GLuint width, GLuint height, Target target=Tex2D, bool bindTexture = true);

    //! Generate openGL mimmap for the texture
    void generateMipmap();

    //! Return openGL texture id
    GLuint	getId() const;

    //! Return openGL texture type
    Type	getType() const;

    //! Set whether or not generate an openGL mipmap
    /*!
      \param use Set whether or not generate an openGL mipmap
     */
    void	setUseMipmap(bool use=true);

    //! Return texture size
    Rectf	getSize() const;

    //! Set openGL texture filter type
    /*!
      \param filterType Filter type
     */
    void	setFilterType(FilterType filterType);

    //! Set openGL texture wrap mode
    /*!
      \param filterType wrap mode
     */
    void	setWrapMode(WrapMode wrap);

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
