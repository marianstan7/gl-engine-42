//
// Octree.hpp for glengine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sat May  5 10:50:13 2012 loick michard
//

#ifndef _OCTREE_HPP_
# define _OCTREE_HPP_

# include <vector>
# include <list>
# include <thread>
# include <mutex>
# include <atomic>
# include <map>
# include <gle/opengl.h>
# include <Vector3.hpp>


namespace gle {
  
  class Material;
  class Mesh;

  //! Handler of %Octree
  class Octree {
  public:
    //! Octree element interface
    class Element
    {
    public:

      //! Destroy octree element
      virtual ~Element() {};

      //! Get the highest extreme value in the 3 axis
      virtual const Vector3<GLfloat>& getMaxPoint() = 0;

      //! Get the lowest extreme value in the 3 axis
      virtual const Vector3<GLfloat>& getMinPoint() = 0;

      //! Get center of octree element
      virtual const Vector3<GLfloat>& getCenter() = 0;

      //! Return wheter or not octree element is in frustum
      /*!
	\param frustum Six planes of frustum
      */
      virtual bool isInFrustum(const GLfloat frustum[6][4]) const = 0;
    };
    
    //! Represent an octree node
    class Node {
    public:
      
      //! Create an octree node
      /*!
	\param min lowest extreme value in the 3 axis
	\param max highest extreme value in the 3 axis
	\param elements Elements totally in frustum
	\param partialsElements Elements partially in frustum
       */
      Node(const Vector3<GLfloat>& min,
	   const Vector3<GLfloat>& max,
	   const std::list<Element*> &elements,
	   const std::list<Element*> &partialsElements);

      //! Destroy an octree node
      ~Node();

      //! Return the node debug mesh
      /*!
	This is used only if renderer debug mode is activated and set to Renderer::Octree
      */
      Mesh* getDebugMesh();

      //! Recursively split node
      /*!
	It recursively split node in 8 children.
	All node elements are divide in each child.
	\param thread Set whether or not threading is activated
	\param nbThreads Number of threads currently run
	\param maxThreads Maximum number of threads
	\param depth Current depht of recursion
       */
      void splitNode(bool thread = false, std::atomic<int> *nbThreads = NULL, int maxThreads=0, int depth=0);

      //! Add element to child frustum
      /*!
	\param frustum Six planes of frustum
	\param elementsInFrustum Elements to check
	\param alreadyDone Elements already add
       */
      void addToFrustum(const GLfloat frustum[6][4], 
			std::list<Element*>& elementsInFrustum,
			std::map<Element*, bool>* alreadyDone);

      //! Octree node children
      Node*		_children[8];

    protected:
      //! Elements totally in frustum
      std::list<Element*> _elements;

      //! Elements partially in frustum
      std::list<Element*> _partialsElements;
    private:
      Material*		_debugMaterial;
      Mesh*		_debugMesh;
      Vector3<GLfloat>	_min;
      Vector3<GLfloat>	_max;
      bool		_leaf;
    };

    //! Create an octree
    Octree();

    //! Destroy octree
    ~Octree();

    //! Generate the octree structure
    /*!
      \param elements Elements to add in octree
     */
    void generateTree(std::list<Element*> &elements);

    //! Get octree debug nodes
    /*!
      This is used only if renderer debug mode is activated and set to Renderer::Octree
    */
    std::vector<Mesh*>& getDebugNodes();

    //! Return a list of all element in given projection and modelview matrix
    /*!
      \param projection Projection matrix of frustum
      \param modelview Modelview matrix of frustum
    */
    std::list<Element*> &getElementsInFrustum(const gle::Matrix4<GLfloat>& projection,
					      const gle::Matrix4<GLfloat>& modelview);

    private:
    void			_addDebugNode(Node* node);
    Node*			_root;
    std::atomic<int>		_nbThreads;
    GLfloat			_frustum[6][4];
    std::list<Element*>		_elementsInFrustum;
    std::map<Element*, bool>	_alreadyDone;
    std::vector<Mesh*>		_debugNodes;
  };
}

#endif
