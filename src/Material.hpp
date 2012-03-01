//
// Material.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 22:30:11 2012 gael jochaud-du-plessix
// Last update Thu Mar  1 17:50:11 2012 gael jochaud-du-plessix
//

#ifndef _GLE_MATERIAL_HPP_
# define _GLE_MATERIAL_HPP_

# include <string>
# include <Program.hpp>
# include <Texture.hpp>
# include <Color.hpp>

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
    Material();

    //! Destruct a material
    ~Material();

    //! Set the material base color

    void setColor(gle::Color<GLfloat> const & color);

    //! Return the material base color

    gle::Color<GLfloat> const & getColor() const;

    //! Return true if the diffuse lighting is enabled

    bool isDiffuseLightEnabled() const;

    //! Return true if the specular lighting is enabled
    bool isSpecularLightEnabled() const;

    //! Return true if either diffuse or specular lighting is enabled

    bool isLightEnabled() const;

    //! Enable or disable diffuse lighting

    bool setDiffuseLightEnabled(bool enabled);

    //! Enable or disable specular lighting

    bool setSpecularLightEnabled(bool enabled);

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

    //! Create the rendering that can be used to render this material
    /*
      \param scene Related scene
    */

    gle::Program* createProgram(Scene* scene);

  private:
    gle::Color<GLfloat> _color;

    bool _diffuseLightEnabled;
    bool _specularLightEnabled;

    GLfloat _shininess;
    GLfloat _diffuseIntensity;
    GLfloat _specularIntensity;

    bool _colorMapEnabled;
    gle::Texture* _colorMap;

    gle::Shader* _createVertexShader(Scene* scene);
    gle::Shader* _createFragmentShader(Scene* scene);
    std::string _replace(std::string const& search,
			 int number,
			 std::string const& str);
  };
}

#endif /* _GLE_MATERIAL_HPP_ */
