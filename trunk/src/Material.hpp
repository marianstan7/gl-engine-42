//
// Material.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 22:30:11 2012 gael jochaud-du-plessix
// Last update Wed Feb 29 17:02:13 2012 loick michard
//

#ifndef _GLE_MATERIAL_HPP_
# define _GLE_MATERIAL_HPP_

# include <Program.hpp>
# include <string>

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

    //! Enable or disable the material color
    void setColorEnabled(bool colorEnabled);

    //! Return true if the color is enabled
    bool isColorEnabled() const;

    //! Set if the material is a light
    /*!
      \param isLight Give true to transform material into a light
    */

    void isLight(bool isLight);

    //! Check if the material is a light

    bool isLight() const;

    //! Set shininess of material
    /*!
      \param shininess Shininess value
    */

    void setShininess(GLfloat shininess);

    //! Get shininess of material

    GLfloat getShininess() const;

    //! Set specular intensity of material
    /*!
      \param specularIntensity Specular intensity value
    */
    
    void setSpecularIntensity(GLfloat specularIntensity);

    //! Get specular intensity of material

    GLfloat getSpecularIntensity() const;    

    //! Create the rendering that can be used to render this material
    /*
      \param scene Related scene
    */

    gle::Program* createProgram(Scene* scene);

  private:
    bool _colorEnabled;
    bool _isLight;
    GLfloat _shininess;
    GLfloat _specularIntensity;

    gle::Shader* _createVertexShader(Scene* scene);
    gle::Shader* _createFragmentShader(Scene* scene);
    std::string _replace(std::string const& search,
			 int number,
			 std::string const& str);
  };
}

#endif /* _GLE_MATERIAL_HPP_ */
