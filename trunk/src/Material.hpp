//
// Material.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 22:30:11 2012 gael jochaud-du-plessix
// Last update Wed May 30 16:56:37 2012 gael jochaud-du-plessix
//

#ifndef _GLE_MATERIAL_HPP_
# define _GLE_MATERIAL_HPP_

# include <string>
# include <Program.hpp>
# include <Texture.hpp>
# include <Color.hpp>
# include <Buffer.hpp>

namespace gle {

  class Scene;

  //! Description of a material
  /*!
    Description and management of a material.
    This class encapsulate the creation and management of the
    OpenGL rendering program associated with the material.
   */

  class Material {
  public:

    //! Create a material
    Material(std::string const & name="");

    //! Destruct a material
    ~Material();

    //! Set the name of the material

    void setName(std::string const & name);

    //! Return the name of the material

    std::string const & getName() const;

    //! Set the material ambient color

    void setAmbientColor(gle::Color<GLfloat> const & color);

    //! Return the material ambient color

    gle::Color<GLfloat> const & getAmbientColor() const;

    //! Set the material diffuse color

    void setDiffuseColor(gle::Color<GLfloat> const & color);

    //! Return the material diffuse color

    gle::Color<GLfloat> const & getDiffuseColor() const;

    //! Set the material specular color

    void setSpecularColor(gle::Color<GLfloat> const & color);

    //! Return the material specular color

    gle::Color<GLfloat> const & getSpecularColor() const;

    //! Return true if the diffuse lighting is enabled

    bool isDiffuseLightEnabled() const;

    //! Return true if the specular lighting is enabled
    bool isSpecularLightEnabled() const;

    //! Enable or disable diffuse lighting

    void setDiffuseLightEnabled(bool enabled);

    //! Enable or disable specular lighting

    void setSpecularLightEnabled(bool enabled);

    //! Set shininess of material
    /*!
      \param shininess Shininess value
    */

    void setShininess(GLfloat shininess);

    //! Get shininess of material

    GLfloat getShininess() const;

    //! Set diffuse intensity of material
    /*!
      \param diffuseIntensity Diffuse intensity value
    */
    
    void setDiffuseIntensity(GLfloat diffuseIntensity);

    //! Set specular intensity of material
    /*!
      \param specularIntensity Specular intensity value
    */
    
    void setSpecularIntensity(GLfloat specularIntensity);

    //! Get diffuse intensity of material

    GLfloat getDiffuseIntensity() const;

    //! Get specular intensity of material

    GLfloat getSpecularIntensity() const;

    //! Return true if the color map is enabled

    bool isColorMapEnabled() const;

    //! Enable or disable the color map

    void setColorMapEnabled(bool enabled);

    //! Get color map texture

    gle::Texture* getColorMap() const;

    //! Set color map texture

    void setColorMap(gle::Texture* colorMap);

    //! Returns the uniform buffer containing all datas for the material
    /*!
      Build the buffer if necessary
     */

    Buffer<GLfloat>* getUniforms();

  private:
    std::string _name;
    
    gle::Color<GLfloat> _ambientColor;
    gle::Color<GLfloat> _diffuseColor;
    gle::Color<GLfloat> _specularColor;

    bool _diffuseLightEnabled;
    bool _specularLightEnabled;

    GLfloat _shininess;
    GLfloat _diffuseIntensity;
    GLfloat _specularIntensity;

    bool _colorMapEnabled;
    gle::Texture* _colorMap;

    Buffer<GLfloat> *_uniforms;
    bool _needUniformsUpdate;
  };
}

#endif /* _GLE_MATERIAL_HPP_ */
