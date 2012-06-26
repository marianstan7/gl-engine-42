//
// Skeleton.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Tue Jun 26 11:20:15 2012 loick michard
// Last update Tue Jun 26 11:50:48 2012 loick michard
//

#ifndef _SKELETON_HPP_
# define _SKELETON_HPP_

#include <Scene.hpp>

namespace gle {

  class Skeleton : public Scene::Node  {
  public:
    Skeleton();
    ~Skeleton();

    void	setId(GLint id);
    GLint	getId() const;
  private:
    GLint	_id;
  };

}
#endif
