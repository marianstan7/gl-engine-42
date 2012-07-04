//
// Material.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 22:30:11 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 19:57:41 2012 loick michard
//

#ifndef _GLE_MATERIAL_HPP_
# define _GLE_MATERIAL_HPP_

# include <string>
# include <Program.hpp>
# include <Texture.hpp>
# include <Color.hpp>
# include <Buffer.hpp>
# include <EnvironmentMap.hpp>
# include <Scene.hpp>

namespace gle {

  //! Description of a material
  /*!
    Description and management of a material.
    This class encapsulate the creation and management of the
    OpenGL rendering program associated with the material.
   */

  class Material {
  public:

    //! Size of the datas used by one material in a uniform buffer

    static const GLsizeiptr UniformSize = 20;

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

    void setReflectionIntensity(GLfloat reflection);

    //! Return reflection intensity
    GLfloat getReflectionIntensity() const;

    //! Return whether or not color map is enabled
    bool isColorMapEnabled() const;

    //! Enable or disable the color map

    void setColorMapEnabled(bool enabled);

    //! Get color map texture

    gle::Texture* getColorMap() const;

    //! Set color map texture

    void setColorMap(gle::Texture* colorMap);

    //! Return whether or not normal map is enabled
    bool isNormalMapEnabled() const;

    //! Set whether or not normal map is enabled
    /*!
      \param enabled If true normal map is enabled
     */
    void setNormalMapEnabled(bool enabled);

    //! Return normal map
    gle::Texture* getNormalMap() const;

    //! Set normal map
    /*!
      \param normalMap Normal map texture
    */
    void setNormalMap(gle::Texture* normalMap);

    //! Return whether or not environment map is enabled
    bool isEnvMapEnabled() const;

    //! Set whether or not environment map is enabled
    /*!
      \param enabled If true environment map is enabled
     */
    void setEnvEnabled(bool enabled);

    //! Set environment map
    /*!
      \param envMap Normal map texture
    */
    void setEnvMap(gle::EnvironmentMap* envMap);

    //! Return environment map
    gle::EnvironmentMap* getEnvMap() const;

    //! Returns the uniform buffer containing all datas for the material
    /*!
      Build the buffer if necessary
     */

    Buffer<GLfloat>* getUniformsBuffer() const;

    //! Returns the data of the material packed for storing in a uniform buffer
    /*!
      Build the datas if necessary
     */

    const GLfloat* getUniforms() const;

    //! Return whether or not two materials are compatible for rendering
    /*!
      /param group Group rendering parameters for check compatibility
    */
    bool canBeRenderedWith(const gle::Scene::MeshGroup& group) const;

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
    GLfloat _reflectionIntensity;

    bool		_colorMapEnabled;
    gle::Texture*	_colorMap;

    bool		_normalMapEnabled;
    gle::Texture*	_normalMap;

    bool			_envMapEnabled;
    gle::EnvironmentMap*	_envMap;

    GLfloat*		_uniforms;
    Buffer<GLfloat>*	_uniformsBuffer;
    mutable bool	_needUniformsUpdate;
    mutable bool	_needUniformsBufferUpdate;
  };
}

#endif /* _GLE_MATERIAL_HPP_ */
