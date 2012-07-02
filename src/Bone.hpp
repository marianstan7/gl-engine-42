//
// Bone.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sun Jun 24 12:15:33 2012 loick michard
// Last update Tue Jun 26 20:26:20 2012 loick michard
//

#ifndef _BONE_HPP_
# define _BONE_HPP_

#include <Scene.hpp>
#include <Mesh.hpp>
#include <Material.hpp>

namespace gle {
  class Bone : public Scene::Node {
  public:
    Bone();
    Bone(GLfloat size);
    ~Bone();
    virtual void update();
    std::vector<Scene::Node*>& getDebugNodes(int mode);
  private:
    GLfloat			_size;
    Material*			_debugMaterial;
    std::vector<Scene::Node*>	_debugMeshes;
  };
}

#endif
