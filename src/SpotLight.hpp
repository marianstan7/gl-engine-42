//
// SpotLight.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 17:36:20 2012 loick michard
// Last update Fri Jul  6 01:41:41 2012 loick michard
//

#ifndef _SPOT_LIGHT_HPP_
# define _SPOT_LIGHT_HPP_

# include <gle/opengl.h>
# include <Light.hpp>
# include <Vector3.hpp>
# include <Color.hpp>

namespace gle {

  //! Spot light class
  /*!
    This class enables to create a spot light, with a position, a color, and a specular color.
  */

  class SpotLight : public Light {
  public:

    //! Construct a spot light
    /*!
      \param position Position of the light
      \param color Color emits by the light
      \param specularColor Specular color emits by the light
      \param cutOff Spot open angle in degree
    */

    SpotLight(Vector3<GLfloat> const& position, Color<GLfloat> const& color,
	      Color<GLfloat> const& specularColor,
	      GLfloat cutOff);

    //! Construct a spot light
    /*!
      This constructor set the specularColor to the same as color
      \param position Position of the light
      \param color Color emits by the light
      \param cutOff Spot open angle in degree
    */

    SpotLight(Vector3<GLfloat> const& position, Color<GLfloat> const& color,
	      GLfloat cutOff);

    //! Default destructor

    ~SpotLight();


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

    //! Set light attenuation
    /*!
      \param constant Constant attenuation
      \param linear Linear attenuation
      \param quadratic Quadratic attenuation
    */
    void setAttenuation(GLfloat constant, GLfloat linear, GLfloat quadratic);

    //! Set spot open angle
    /*
      \param cosCutOff Spot open angle in degree
    */
    void setCutOff(GLfloat cutOff);

    //! Set spot inner open angle
    /*
      \param cosCutOff Spot inner open angle in degree
    */
    void setInnerCutOff(GLfloat innerCutOff);

    //! Get the light color

    GLfloat* getColor();

    //! Get the light specular color

    GLfloat* getSpecularColor();

    //! Return light attenuation
    GLfloat* getAttenuation();

    //! Return spot open angle in degree
    GLfloat getCutOff();

    //! Return spot inner open angle in degree
    GLfloat getInnerCutOff();

    //! Return spot cos cut off
    GLfloat getCosCutOff();

    //! Return spot inner cos cut off
    GLfloat getInnerCosCutOff();

    //! Return shadow map camera
    gle::Camera* getShadowMapCamera();

    //! Update transformation
    void update();

  private:
    GLfloat	_color[3];
    GLfloat	_specularColor[3];
    GLfloat	_attenuation[3];
    GLfloat	_direction[3];
    GLfloat	_cosCutOff;
    GLfloat	_innerCosCutOff;
  };
}

#endif // _SPOT_LIGHT_HPP_
