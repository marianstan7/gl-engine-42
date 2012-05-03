//
// Scene.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 18:18:01 2012 gael jochaud-du-plessix
// Last update Thu May  3 15:49:08 2012 gael jochaud-du-plessix
//

#ifndef _GLE_SCENE_HPP_
# define _GLE_SCENE_HPP_

# include <vector>
# include <map>
# include <Camera.hpp>
# include <Mesh.hpp>
# include <Program.hpp>
# include <Material.hpp>
# include <Light.hpp>
# include <Color.hpp>

namespace gle {

  //! Describe a 3D scene
  /*!
    Container for all the elements and caracteristics of a 3D scene.
    (Cameras, meshes, env map, etc...)
   */

  class Scene {
  public:
    //! Construct a scene
    /*!
      Construct an empty scene, ready to contain cameras and meshes
    */

    Scene();

    //! Destruct a scene
    ~Scene();

    void setBackgroundColor(gle::Color<GLfloat> const &color);

    gle::Color<GLfloat> const & getBackgroundColor() const;

    //! Add a camera to the scene
    /*!
      If the camera is already part of the scene, it's not added.
      \param camera A pointer to the Camera object to add to the scene
      \return A reference to the scene
      \sa add(Mesh *mesh)
     */

    Scene & add(Camera *camera);

    //! Add a mesh to the scene
    /*!
      If the mesh is already part of the scene, it's not added.
      This function recursively add all the children of the mesh.
      \param mesh A pointer to the Mesh object to add to the scene
      \return A reference to the scene
     */

    Scene & add(Mesh *mesh);

    //! Add a list of meshes to the scene
    /*!
      If the one of the mesh is already part of the scene, it's not added.
      This function recursively add all the children of all the meshes.
      \param meshes A std::vector of pointers to thes meshes to be added
      \return A reference to the scene
     */

    Scene & add(std::vector<gle::Mesh*> const & meshes);

    //! Add a material to the scene
    /*!
      If the material is already part of the scene, it's not added.
      \param material A pointer to the Material object to add to the scene
      \return A reference to the scene
    */

    Scene & add(Material *material);

    //! Add a light to the scene
    /*!
      If the light is already part of the scene, it's not added.
      \param light A pointer to the Light object to add to the scene
      \return A reference to the scene
    */

    Scene & add(Light *light);

    //! Remove a camera from the scene
    /*!
      If the camera isn't in the scene, nothing is done.
      \param camera A pointer to the Camera object to remove from the scene
      \return A reference to the scene
      \sa remove(Mesh *mesh), remove(Material* material)
     */

    Scene & remove(Camera *camera);

    //! Remove a mesh from the scene
    /*!
      If the mesh isn't in the scene, nothing is done.
      \param mesh A pointer to the Mesh object to remove from the scene
      \return A reference to the scene
      \sa remove(Camera *camera), remove(Material* material)
     */

    Scene & remove(Mesh *mesh);

    //! remove a material from the scene
    /*!
      If the material isn't in the scene, nothing is done.
      \param material A pointer to the Material object to remove from the scene
      \return A reference to the scene
      \sa remove(Mesh* mesh), remove(Camera *camera)
     */

    Scene & remove(Material *material);

    //! remove a light from the scene
    /*!
      If the light isn't in the scene, nothing is done.
      \param light A pointer to the Light object to remove from the scene
      \return A reference to the scene
      \sa remove(Mesh* mesh), remove(Camera *camera)
     */

    Scene & remove(Light *light);
    
    //! Add an element to the scene
    /*!
      \param element An element to add to the scene
      \return A reference to the scene
      \sa add(Camera *camera), add(Mesh *mesh)
    */
    template <typename T>
    Scene & operator<<(T element)
    {
      return (add(element));
    }

    //! Get a vector of all meshes which are in the scene

    std::vector<Mesh*> & getMeshes();

    //! Get a vector of all cameras which are in the scene

    std::vector<Camera*> & getCameras();

    //! Get a vector of all materials which are in the scene

    std::vector<Material*> & getMaterials();

    //! Get a vector of all lights which are in the scene

    std::vector<Light*> & getLights();

    //! Get a pointer to the current camera

    gle::Camera* getCurrentCamera();

    //! Get a map of all Programs associate with their material

    std::map<Material*, Program*> & getPrograms();

    //! Get a GLfloat tab of all directional lights direction

    GLfloat* getDirectionalLightsDirection() const;

    //! Get a GLfloat tab of all directional lights color

    GLfloat* getDirectionalLightsColor() const;

    //! Get number of directional lights in the scene

    GLsizeiptr getDirectionalLightsSize() const;

    //! Get a GLfloat tab of all point lights direction

    GLfloat* getPointLightsPosition() const;

    //! Get a GLfloat tab of all point lights color

    GLfloat* getPointLightsColor() const;

    //! Get a GLfloat tab of all point lights specular color

    GLfloat* getPointLightsSpecularColor() const;

    //! Get number of point lights in the scene

    GLsizeiptr getPointLightsSize() const;

    //! Return true if there are lights in the scene (point or directional)

    bool hasLights() const;

    //! Update lights
    /*!
      You have to call this function when you change any light color or position
    */
    void updateLights();

    void buildProgram();

    gle::Program* getProgram();

  private:
    gle::Shader*		_createVertexShader();
    gle::Shader*		_createFragmentShader();
    std::string			_replace(std::string const& search, int number, std::string const& str);
    
    gle::Color<GLfloat>		_backgroundColor;
    std::vector<Camera*>	_cameras;
    std::vector<Mesh*>		_meshes;
    std::vector<Material*>	_materials;
    std::vector<Light*>		_lights;

    std::vector<GLfloat>	_directionalLightsDirection;
    std::vector<GLfloat>	_directionalLightsColor;
    GLsizeiptr			_directionalLightsSize;

    std::vector<GLfloat>	_pointLightsPosition;
    std::vector<GLfloat>	_pointLightsColor;
    std::vector<GLfloat>	_pointLightsSpecularColor;
    GLsizeiptr			_pointLightsSize;

    Camera*			_currentCamera;
    gle::Program*		_program;
    bool			_needProgramCompilation;

    std::vector<Mesh*>		_meshesToRender;
    std::map<Mesh*, GLuint>	_meshUniformBlockIndexes;
  };
}

#endif /* _GLE_SCENE_HPP_ */
