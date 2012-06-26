//
// Scene.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 18:18:01 2012 gael jochaud-du-plessix
// Last update Tue Jun 26 14:51:07 2012 loick michard
//

#ifndef _GLE_SCENE_HPP_
# define _GLE_SCENE_HPP_

# include <vector>
# include <list>
# include <map>
# include <utility>
# include <Program.hpp>
# include <Color.hpp>
# include <Quaternion.hpp>
# include <EnvironmentMap.hpp>
# include <Buffer.hpp>
# include <Octree.hpp>

namespace gle {

  class Camera;
  class Light;
  class Mesh;
  class Texture;
  class Material;
  class Bone;

  //! Describe a 3D scene
  /*!
    Container for all the elements and caracteristics of a 3D scene.
    (Cameras, meshes, env map, etc...)
   */

  class Scene {
  public:

    //! Represents a scene node
    /*!
      Nodes are the interface for all elements that a scene can contain.
      They share several properties like a position, a rotation, etc...
      They can also be retrieved thanks to their name.
      Scene nodes have a pointer to their parent and a list of their childs
      in order to easily retrieve them in the scene graph.
     */

    class Node {
    public:

      //! Different types of scene nodes
      enum Type {
	//! An empty node, used as a container for other nodes
	Empty,
	//! Node representing a mesh object
	Mesh,
	//! Node representing a camera
	Camera,
	//! Node representing a light
	Light,
	//! Node representing a bone
	Bone,
	//! Node representing a skeleton
        Skeleton
      };

      //! Constructs a node with its type

      Node(Type type = Empty);

      //! Copy a node
      /*!
	Copy a node and duplicate all its children recursively
       */

      Node(const Node& other);

      //! Destructs a node

      virtual	~Node();

      //! Returns the type of the node

      Type	getType() const;

      //! Returns the name of the node

      const	std::string& getName() const;

      //! Set the name of the node

      void	setName(const std::string& name);

      //! Set the transformation and the normal matrix of the node

      void	setMatrices(Matrix4<GLfloat> &transformationMatrix, Matrix3<GLfloat> &normalMatrix);

      //! Add a child to the node

      void	addChild(Node* child);

      //! Remove a child from the node

      void	removeChild(Node* child);

      //! Get the list of children of the node

      const std::vector<Node*>&	getChildren() const;

      //! Recursiveley search for child nodes matching a given type and name
      /*!
	This function recursively search for all nodes that are of type T and
	match the given name (i.e that have the substring 'name' in their own)
	and add them to a given vector.
	It returns the number of children found.
	\param name Name to search for
	\param vector Vector that will be filled with matching children
	\return The number of children added to the vector
       */

      template <typename T>
      int	getChildrenByName(std::string const & name,
				  std::vector<T*> & vector)
      {
	int	nb = 0;

	if (_name.find(name) != std::string::npos)
	  {
	    T* element = dynamic_cast<T*>(this);
	    if (element)
	      {
		vector.push_back(element);
		++nb;
	      }
	  }
	for (gle::Scene::Node* &child : _children)
	  nb+= child->getChildrenByName(name, vector);
	return (nb);
      }

      //! Recursiveley search for child nodes matching a given name
      /*!
	This function recursively search for all nodes that match the given name
	(i.e that have the substring 'name' in their own) and add them to a given vector.
	It returns the number of children found.
	\param name Name to search for
	\param vector Vector that will be filled with matching children
	\return The number of children added to the vector
       */

      int	getChildrenByName(std::string const & name, std::vector<Node*> & vector);

      //! Returns the first child matching the given name

      Node*	getChildByName(std::string const & name);

      //! Returns the parent of the node

      void	setParent(Node* parent);

      //! Returns the transformation matrix of the node

      const Matrix4<GLfloat>& getTransformationMatrix();

      //! Returns the normal matrix of the node

      const Matrix3<GLfloat>& getNormalMatrix();

      //! Update the matrices of the node

      virtual void	updateMatrix();

      //! Set the position of the node, relative to its parent

      void	setPosition(const Vector3<GLfloat>& pos);

      //! Set the rotation of the node

      void	setRotation(const Quaternion<GLfloat>& rotation);

      //! Set the rotation of the node using any of the constructor of a quaternion

      template <class... Args>
      void	setRotation(const Args&... args)
      {
	this->setRotation(Quaternion<GLfloat>(args...));
      }

      //! Get the position of the node

      const Vector3<GLfloat>& getPosition() const;

      //! Get the absolute position of the node
      /*!
	Nodes position are relative to their parents.
	The absolute position represent the final position of the node
	in the 3D world
       */

      const Vector3<GLfloat>& getAbsolutePosition() const;

      //! Return true if the node is dynamic

      bool	isDynamic() const;

      //! Set if the node is dynamic or not

      void	setDynamic(bool dynamic, bool deep=true);

      //! Get the target of the node if it is looking at something

      const Vector3<GLfloat>& getTarget() const;

      //! Set the target of the node so it will 'look at' it

      void	setTarget(const Vector3<GLfloat>& target);
      
      //! Set the scale of the node

      void	setScale(GLfloat scale);

      //! Set the scale of the node on the 3 axis

      void	setScale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);

      //! Translate the node

      void	translate(const Vector3<GLfloat>& vec);

      //! Rotate the node

      void	rotate(const Quaternion<GLfloat>& rotation);

      //! Rotate the node with any arguments that can take the quaternion constructor

      template <class... Args>
      void      rotate(const Args&... args)
      {
        this->rotate(Quaternion<GLfloat>(args...));
      }

      //! Duplicate the node

      virtual Node* duplicate() const;

      //! Update the node

      virtual void update();

      virtual std::vector<Node*>& getDebugNodes(int mode);

    protected:

      //! Type of the node
      const Type		_type;

      //! Name of the node
      std::string		_name;

      //! List of pointers to children of the node
      std::vector<Node*>	_children;

      //! Pointer to the parent node
      Node*			_parent;

      //! Position of the node
      Vector3<GLfloat>		_position;

      //! Absolute position of the node
      Vector3<GLfloat>		_absolutePosition;

      //! Define if the node is dynamic or not
      bool			_isDynamic;

      //! Target of the node
      Vector3<GLfloat>		_target;

      //! Specify wether the node has a target or not
      bool			_hasTarget;

      //! Scale matrix of the node
      Matrix4<GLfloat>		_scaleMatrix;

      //! Rotation matrix of the node
      Matrix4<GLfloat>		_rotationMatrix;
      
      //! Transformation matrix of the node
      Matrix4<GLfloat>		_transformationMatrix;
      
      //! Specific transformation matrix for camera nodes
      Matrix4<GLfloat>		_cameraTransformationMatrix;

      //! Normal matrix
      Matrix3<GLfloat>		_normalMatrix;

      std::vector<Node*>	_debugNodes;
    };

    //! Symbolize a group of meshes for rendering

    struct MeshGroup {
      std::list<Mesh*>          meshes;
      GLint			uniformBufferId;
      GLint			materialBufferId;
      gle::Texture*             colorMap;
      gle::Texture*             normalMap;
      gle::EnvironmentMap*      envMap;
    };

    //! Constructs a scene
    /*!
      Constructs an empty scene, ready to contain all types of scene nodes
    */

    Scene();

    //! Destruct a scene

    ~Scene();

    //! Set the background color of the scene

    void setBackgroundColor(gle::Color<GLfloat> const &color);

    //! Return the background color of the scene

    gle::Color<GLfloat> const & getBackgroundColor() const;

    //! Set the fog color of the scene
    /*!
      Most of the time, it's better to set the fog to the same color
      as the background.
     */
    void setFogColor(gle::Color<GLfloat> const &color);

    //! Returns the fog color

    gle::Color<GLfloat> const & getFogColor() const;

    //! Set the density of the fog in the scene
    /*!
      The fog density determines the minimum distance where the fog will
      appear.
      1 is a totally opaque fog, making the scene filled with the fog color.
      0 is a totally transparent fog, disabling its effects.
      Depending on your scene and the atmosphere you want to render,
      you can set the density of the fog. Most of the time, you should
      use little values under 0.1.
     */

    void setFogDensity(GLfloat density);

    //! Returns the density of the fog

    GLfloat getFogDensity() const;

    //! Add a node to the scene (add it to the root node)

    Scene & add(Node* element);

    //! Add a vector of nodes to the scene (add them to the root node)

    Scene & add(std::vector<Node*> elements);

    //! Remove a node from the scene

    Scene & remove(Node* element);

    //! Returns a reference to the root node of the scene

    Node& getRootNode();

    //! Add a node to the scene

    Scene & operator<<(Node* element)
    {
      return (this->add(element));
    }

    //! Add a list of nodes to the scene
    template <typename T>
    Scene & operator<<(std::vector<T*> elements)
    {
      for (T* &element : elements)
	this->add(element);
      return (*this);
    }

    //! Get a vector of all static meshes
    /*
      If frustum culling is enabled, the vector only contains meshes
      that are in the frustum of the current camera.
     */

    const std::vector<Mesh*> & getStaticMeshes();

    //! Get a vector of all dynamic meshes

    const std::vector<Mesh*> & getDynamicMeshes();

    //! Process the frustum culling on the list of meshes to render

    void processFrustumCulling();

    //! Get a vector of all cameras which are in the scene

    std::vector<Camera*> & getCameras();

    //! Get a vector of all lights which are in the scene

    const std::vector<Light*> & getLights() const;

    //! Get a pointer to the current camera

    gle::Camera* getCurrentCamera();

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

    GLfloat* getPointLightsAttenuation() const;

    //! Get number of point lights in the scene

    GLsizeiptr getPointLightsSize() const;

    //! Get a GLfloat tab of all point lights direction

    GLfloat* getSpotLightsPosition() const;

    //! Get a GLfloat tab of all spot lights color

    GLfloat* getSpotLightsColor() const;

    //! Get a GLfloat tab of all spot lights specular color

    GLfloat* getSpotLightsSpecularColor() const;

    GLfloat* getSpotLightsAttenuation() const;

    GLfloat* getSpotLightsDirection() const;

    GLfloat* getSpotLightsCosCutOff() const;

    //! Get number of spot lights in the scene

    GLsizeiptr getSpotLightsSize() const;


    //! Return true if there are lights in the scene (point or directional)

    bool hasLights() const;

    //! Update lights
    /*!
      You have to call this function when you change any light color or position
    */

    void updateLights();

    void updateBones();

    //! Update dynamic meshes
    /*
      This function has to be called when dynamic meshes are aded or removed form the scene.
     */

    void updateDynamicMeshes();

    //! Update static meshes
    /*!
      You have to call this function when you add, remove or update
      any static mesh in the scene
      This function re-generates the mesh uniforms buffer and recompile
      the shader so it is very CPU intensive
    */

    void updateStaticMeshes();

    //! Builds the shader program for the scene

    void buildProgram();

    //! Returns the shader program for the

    gle::Program* getProgram();

    //! Display the bounding volumes of the scene nodes when rendering

    void displayBoundingVolume();

    //! Generates the spatial partitionning tree of the scene

    void generateTree();

    //! Enable or disable the frustum culling in the scene

    void enableFrustumCulling(bool enable = true);

    //! Set the camera used to render the scene. By default, use the first camera added.

    void setCamera(Camera* camera);

    //! Update the scene. Must be called after any modification to the scene graph.
    
    void update(Node* node=NULL, int depth = 0);

    void setEnvMap(EnvironmentMap* envMap);
    void setEnvMapEnabled(bool enabled);
    EnvironmentMap* getEnvMap() const;
    bool isEnvMapEnabled() const;
    Program* getEnvMapProgram() const;
    Mesh* getEnvMapMesh() const;

    //! Returns the uniform buffer containing static meshes matrices

    const gle::Bufferf*   getStaticMeshesUniformsBuffer(GLuint bufferId) const;

    //! Returns the uniform buffer containing static meshes materials

    const gle::Bufferf*     getStaticMeshesMaterialsBuffer(GLuint bufferId) const;

    std::vector<Node*>& getDebugNodes(int mode);

  private:
    gle::Shader*	_createVertexShader();
    gle::Shader*	_createFragmentShader();
    std::string		_replace(std::string const& search, int number,
				 std::string const& str);

    void		_buildMaterialBuffers(std::list<MeshGroup>&, GLint);
    void		_clearStaticMeshesBuffers();

    gle::Color<GLfloat>	_backgroundColor;
    gle::Color<GLfloat>	_fogColor;
    GLfloat		_fogDensity;

    std::vector<Camera*>	_cameras;
    std::vector<Mesh*>		_staticMeshes;
    std::vector<Mesh*>		_dynamicMeshes;
    std::vector<Mesh*>		_meshesInFrustum;

    std::vector<Light*>	_lights;
    gle::Scene::Node	_root;

    std::vector<GLfloat>	_directionalLightsDirection;
    std::vector<GLfloat>	_directionalLightsColor;
    GLsizeiptr			_directionalLightsSize;

    std::vector<GLfloat>	_pointLightsPosition;
    std::vector<GLfloat>	_pointLightsColor;
    std::vector<GLfloat>	_pointLightsSpecularColor;
    std::vector<GLfloat>	_pointLightsAttenuation;
    GLsizeiptr			_pointLightsSize;

    std::vector<GLfloat>	_spotLightsPosition;
    std::vector<GLfloat>	_spotLightsColor;
    std::vector<GLfloat>	_spotLightsSpecularColor;
    std::vector<GLfloat>	_spotLightsAttenuation;
    std::vector<GLfloat>	_spotLightsDirection;
    std::vector<GLfloat>	_spotLightsCosCutOff;
    GLsizeiptr			_spotLightsSize;

    gle::Camera*	_currentCamera;

    gle::Program*	_program;
    bool		_needProgramCompilation;

    std::vector<gle::Bufferf*>				_staticMeshesUniformsBuffers;
    std::vector<gle::Bufferf*>				_staticMeshesMaterialsBuffers;
    std::map<gle::Material*, std::pair<GLuint, GLuint>>	_staticMeshesMaterialsBuffersIds;

    Octree	_tree;
    bool	_frustumCulling;

    EnvironmentMap*	_envMap;
    bool		_isEnvMapEnabled;
    Program*		_envMapProgram;
    Mesh*		_envMapMesh;

    std::vector<Node*> _debugNodes;
    void		_addDebugNodes(Scene::Node* node, int mode);

    std::vector<Bone*>		_bones;
    std::vector<GLfloat>	_bonesMatrices;
  };
}

#endif /* _GLE_SCENE_HPP_ */
