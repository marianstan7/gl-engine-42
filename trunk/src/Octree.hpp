//
// Octree.hpp for glengine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sat May  5 10:50:13 2012 loick michard
// Last update Tue Jun 26 18:57:11 2012 gael jochaud-du-plessix
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

  class Octree {
  public:
    
    class Element
    {
    public:
      virtual ~Element() {};
      virtual const Vector3<GLfloat>& getMaxPoint() const = 0;
      virtual const Vector3<GLfloat>& getMinPoint() const = 0;
      virtual const Vector3<GLfloat>& getCenter() const = 0;
      virtual bool isInFrustum(const GLfloat frustum[6][4]) const = 0;
    };
    
    class Node {
    public:
      Node(const Vector3<GLfloat>& min,
	   const Vector3<GLfloat>& max,
	   const std::list<Element*> &elements,
	   const std::list<Element*> &partialsElements);
      ~Node();

      Mesh* getDebugMesh();
      void splitNode(bool thread = false, std::atomic<int> *nbThreads = NULL, int maxThreads=0, int depth=0);
      void addToFrustum(const GLfloat frustum[6][4], 
			std::list<Element*>& elementsInFrustum,
			std::map<Element*, bool>* alreadyDone);
      Node*		_children[8];
    protected:
      std::list<Element*> _elements;
      std::list<Element*> _partialsElements;
    private:
      Material*		_debugMaterial;
      Mesh*		_debugMesh;
      Vector3<GLfloat>	_min;
      Vector3<GLfloat>	_max;
      bool		_leaf;
    };

    Octree();
    ~Octree();
    void generateTree(std::list<Element*> &elements);
    std::vector<Mesh*>& getDebugNodes();
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
