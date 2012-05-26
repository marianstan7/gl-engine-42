//
// Scene.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 18:18:01 2012 gael jochaud-du-plessix
// Last update Sat May 26 18:31:33 2012 loick michard
//

#ifndef _GLE_SCENE_HPP_
# define _GLE_SCENE_HPP_

# include <vector>
# include <map>
# include <Program.hpp>
# include <Material.hpp>
# include <Color.hpp>
# include <Octree.hpp>
# include <Quaternion.hpp>

namespace gle {

  //! Describe a 3D scene
  /*!
    Container for all the elements and caracteristics of a 3D scene.
    (Cameras, meshes, env map, etc...)
   */

  class Camera;
  class Light;

  class Scene {
  public:

    class Node {
    public:
      enum Type {
	Empty,
	Mesh,
	Camera,
	Light
      };
      Node(Type type = Empty);
      Node(const Node& other);
      virtual	~Node();

      Type	getType() const;
      const	std::string& getName() const;
      void	setName(const std::string& name);
      void	setMatrices(Matrix4<GLfloat> &transformationMatrix, Matrix3<GLfloat> &normalMatrix);

      void	addChild(Node* child);
      void	removeChild(Node* child);
      const std::vector<Node*>&	getChildren() const;
      template <typename T>
      int	getChildrenByName(std::string const & name,
				  std::vector<T*> & vector)
      {
	if (_name == name)
	  {
	    T* element = dynamic_cast<T*>(this);
	    if (element)
	      vector.push_back(element);
	  }
	for (gle::Scene::Node* &child : _children)
	  child->getChildrenByName(name, vector);
      }

      Node*	getChildByName(std::string const & name);
      void	setParent(Node* parent);

      const Matrix4<GLfloat>& getTransformationMatrix();
      const Matrix3<GLfloat>& getNormalMatrix();
      void	updateMatrix();

      void	setPosition(const Vector3<GLfloat>& pos);
      void	setRotation(const Quaternion<GLfloat>& rotation);
      template <class... Args>
      void	setRotation(const Args&... args)
      {
	this->setRotation(Quaternion<GLfloat>(args...));
      }

      const Vector3<GLfloat>& getPosition() const;
      const Vector3<GLfloat>& getAbsolutePosition() const;
      const Vector3<GLfloat>& getTarget() const;

      void	setTarget(const Vector3<GLfloat>& target);
      void	setScale(GLfloat scale);
      void	setScale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);

      void	translate(const Vector3<GLfloat>& vec);
      void	rotate(const Quaternion<GLfloat>& rotation);
      template <class... Args>
      void      rotate(const Args&... args)
      {
        this->rotate(Quaternion<GLfloat>(args...));
      }

      virtual Node* duplicate() const;
      virtual void update();

    protected:
      const Type		_type;
      std::string		_name;
      std::vector<Node*>	_children;
      Node*			_parent;

      Vector3<GLfloat>		_position;
      Vector3<GLfloat>		_absolutePosition;
      Vector3<GLfloat>		_target;
      bool			_hasTarget;
      Matrix4<GLfloat>		_scaleMatrix;
      Matrix4<GLfloat>		_rotationMatrix;
      
      Matrix4<GLfloat>		_transformationMatrix;
      Matrix4<GLfloat>		_cameraTransformationMatrix;
      Matrix3<GLfloat>		_normalMatrix;

    };

    //! Construct a scene
    /*!
      Construct an empty scene, ready to contain cameras and meshes
    */

    Scene();

    //! Destruct a scene
    ~Scene();

    void setBackgroundColor(gle::Color<GLfloat> const &color);

    gle::Color<GLfloat> const & getBackgroundColor() const;

    Scene & add(Node* element);
    Scene & add(std::vector<Node*> elements);
    Scene & remove(Node* element);

    //! Add a camera to the scene
    /*!
      If the camera is already part of the scene, it's not added.
      \param camera A pointer to the Camera object to add to the scene
      \return A reference to the scene
      \sa add(Mesh *mesh)
     */

    //Scene & add(Camera *camera);

    //! Add a mesh to the scene
    /*!
      If the mesh is already part of the scene, it's not added.
      This function recursively add all the children of the mesh.
      \param mesh A pointer to the Mesh object to add to the scene
      \return A reference to the scene
     */

    //Scene & add(Mesh *mesh);

    //! Add a list of meshes to the scene
    /*!
      If the one of the mesh is already part of the scene, it's not added.
      This function recursively add all the children of all the meshes.
      \param meshes A std::vector of pointers to thes meshes to be added
      \return A reference to the scene
     */

    //Scene & add(std::vector<gle::Mesh*> const & meshes);

    //! Add a material to the scene
    /*!
      If the material is already part of the scene, it's not added.
      \param material A pointer to the Material object to add to the scene
      \return A reference to the scene
    */

    //Scene & add(Material *material);

    //! Add a light to the scene
    /*!
      If the light is already part of the scene, it's not added.
      \param light A pointer to the Light object to add to the scene
      \return A reference to the scene
    */

    //Scene & add(Light *light);

    //! Remove a camera from the scene
    /*!
      If the camera isn't in the scene, nothing is done.
      \param camera A pointer to the Camera object to remove from the scene
      \return A reference to the scene
      \sa remove(Mesh *mesh), remove(Material* material)
     */

    //Scene & remove(Camera *camera);

    //! Remove a mesh from the scene
    /*!
      If the mesh isn't in the scene, nothing is done.
      \param mesh A pointer to the Mesh object to remove from the scene
      \return A reference to the scene
      \sa remove(Camera *camera), remove(Material* material)
     */

    //Scene & remove(Mesh *mesh);

    //! remove a material from the scene
    /*!
      If the material isn't in the scene, nothing is done.
      \param material A pointer to the Material object to remove from the scene
      \return A reference to the scene
      \sa remove(Mesh* mesh), remove(Camera *camera)
     */

    //Scene & remove(Material *material);

    //! remove a light from the scene
    /*!
      If the light isn't in the scene, nothing is done.
      \param light A pointer to the Light object to remove from the scene
      \return A reference to the scene
      \sa remove(Mesh* mesh), remove(Camera *camera)
     */

    //Scene & remove(Light *light);
    
    //! Add an element to the scene
    /*!
      \param element An element to add to the scene
      \return A reference to the scene
      \sa add(Camera *camera), add(Mesh *mesh)
    */

    Scene & operator<<(Node* element)
    {
      return (this->add(element));
    }

    template <typename T>
    Scene & operator<<(std::vector<T*> elements)
    {
      for (T* &element : elements)
	this->add(element);
      return (*this);
    }

    //! Get a vector of all meshes which are in the scene

    const std::vector<Mesh*> & getMeshesToRender();

    const std::vector<Mesh*> & getUnboundingMeshesToRender();

    //! Get a vector of all cameras which are in the scene

    std::vector<Camera*> & getCameras();

    //! Get a vector of all materials which are in the scene

    //std::vector<Material*> & getMaterials();

    //! Get a vector of all lights which are in the scene

    const std::vector<Light*> & getLights() const;

    //! Get a pointer to the current camera

    gle::Camera* getCurrentCamera();

    //! Get a map of all Programs associate with their material

    //std::map<Material*, Program*> & getPrograms();

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

    void displayBoundingVolume();

    void generateTree();
    void displayTree();

    void enableFrustumCulling(bool enable = true);

    void setCamera(Camera* camera);
    
    void updateScene(Node* node=NULL, int depth = 0);

  private:
    gle::Shader* _createVertexShader();
    gle::Shader* _createFragmentShader();
    std::string _replace(std::string const& search,
                         int number,
                         std::string const& str);

    gle::Color<GLfloat>	_backgroundColor;

    std::vector<Camera*> _cameras;
    std::vector<Mesh*> _meshesToRender;
    std::vector<Mesh*> _unboundingMeshesToRender;
    //std::vector<Material*> _materials;
    std::vector<Light*> _lights;
    Node _root;

    std::vector<GLfloat> _directionalLightsDirection;
    std::vector<GLfloat> _directionalLightsColor;
    GLsizeiptr _directionalLightsSize;

    std::vector<GLfloat> _pointLightsPosition;
    std::vector<GLfloat> _pointLightsColor;
    std::vector<GLfloat> _pointLightsSpecularColor;
    GLsizeiptr _pointLightsSize;    

    Camera* _currentCamera;
    gle::Program* _program;
    bool _needProgramCompilation;

    bool _displayBoundingVolume;

    Octree _tree;

    bool _frustumCulling;
  };
}

#endif /* _GLE_SCENE_HPP_ */
