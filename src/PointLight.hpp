//
// PointLight.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 17:36:20 2012 loick michard
// Last update Mon Jun  4 14:56:41 2012 loick michard
//

#ifndef _POINT_LIGHT_HPP_
# define _POINT_LIGHT_HPP_

# include <gle/opengl.h>
# include <Light.hpp>
# include <Vector3.hpp>
# include <Color.hpp>

namespace gle {

  //! Point light class
  /*!
    This class enables to create a point light, with a position, a color, and a specular color.
  */

  class PointLight : public Light {
  public:

    //! Construct a point light
    /*!
      \param position Position of the light
      \param color Color emits by the light
      \param specularColor Specular color emits by the light
    */

    PointLight(Vector3<GLfloat> const& position, Color<GLfloat> const& color,
	       Color<GLfloat> const& specularColor);

    //! Construct a point light
    /*!
      This constructor set the specularColor to the same as color
      \param position Position of the light
      \param color Color emits by the light
    */

    PointLight(Vector3<GLfloat> const& position, Color<GLfloat> const& color);

    //! Default destructor

    ~PointLight();


    //! Set the light color
    /*!
      \param color New light color
    */

    void setColor(Color<GLfloat> const& color);

    //! Set the light specular color
    /*!
      \param specularColor New light specular color
    */

    void setSpecularColor(Color<GLfloat> const& specularColor);

    void setAttenuation(GLfloat constant, GLfloat linear, GLfloat quadratic);

    //! Get the light color

    GLfloat* getColor();

    //! Get the light specular color

    GLfloat* getSpecularColor();

    GLfloat* getAttenuation();

    virtual void update();

  private:
    GLfloat _color[3];
    GLfloat _specularColor[3];
    GLfloat _attenuation[3];
  };
}

#endif // _POINT_LIGHT_HPP_
