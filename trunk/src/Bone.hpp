//
// Bone.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sun Jun 24 12:15:33 2012 loick michard
// Last update Wed Jul  4 18:55:35 2012 loick michard
//

#ifndef _BONE_HPP_
# define _BONE_HPP_

#include <Scene.hpp>
#include <Mesh.hpp>
#include <Material.hpp>

namespace gle {
  //! Handler bone structure
  class Bone : public Scene::Node {
  public:

    //! Create a bone
    Bone();

    //! Create a bone
    /*!
      \param size Size of the bone
      Set size for debug representation.
    */
    Bone(GLfloat size);

    //! Destroy bone
    ~Bone();

    //! Update bone transformation
    virtual void update();

    //! Get bone debug nodes
    /*!
      \param mode mode of debugging
       Return empty vector if mode != Renderer::Octree
     */
    std::vector<Scene::Node*>& getDebugNodes(int mode);
  private:
    GLfloat			_size;
    Material*			_debugMaterial;
    std::vector<Scene::Node*>	_debugMeshes;
  };
}

#endif
