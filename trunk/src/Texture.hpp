//
// Texture.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 29 19:37:51 2012 gael jochaud-du-plessix
// Last update Thu Mar  1 13:56:21 2012 gael jochaud-du-plessix
//

#ifndef _GLE_TEXTURE_HPP_
# define _GLE_TEXTURE_HPP_

# include <SFML/Graphics/Image.hpp>
# include <string>
# include <gle/opengl.h>

namespace gle {

  class Texture {
  public:
    enum Type {
      Texture2D = GL_TEXTURE_2D
    };

    Texture(std::string filename, Type type=Texture2D);

    ~Texture();

    void bind();
    
    void setData(std::string filename);
    void setData(sf::Image const & image);

  private:
    GLuint _id;
    Type _type;
  };

}

#endif /* _GLE_TEXTURE_HPP_ */
