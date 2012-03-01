//
// DirectionalLight.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 17:36:20 2012 loick michard
// Last update Wed Feb 29 17:09:16 2012 loick michard
//

#ifndef _DIRECTIONAL_LIGHT_HPP_
# define _DIRECTIONAL_LIGHT_HPP_

# include <gle/opengl.h>
# include <Light.hpp>
# include <Vector3.hpp>
# include <Color.hpp>

namespace gle {

  //! Directional light class
  /*!
    This class enables to create a directional light, with a direction, and a color
  */
  
  class DirectionalLight : public Light {
  public:

    //! Construct a directional light
    /*!
      \param direction Light direction
      \param color Light color
    */

    DirectionalLight(Vector3<GLfloat> const& direction, Color<GLfloat> const& color);

    //! Default destructor

    ~DirectionalLight();

    //! Set light direction
    /*!
      \param direction New light direction
    */

    void setDirection(Vector3<GLfloat> const& direction);

    //! Set light color
    /*!
      \param color New light color
    */

    void setColor(Color<GLfloat> const& color);
    
    //! Get light direction
    
    Vector3<GLfloat>& getDirection();

    //! Get light color

    GLfloat* getColor();
  private:
    Vector3<GLfloat> _direction;
    GLfloat _color[3];
  };
}

#endif // _DIRECTIONAL_LIGHT_HPP_
