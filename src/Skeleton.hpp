//
// Skeleton.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Tue Jun 26 11:20:15 2012 loick michard
// Last update Wed Jul  4 19:08:03 2012 loick michard
//

#ifndef _SKELETON_HPP_
# define _SKELETON_HPP_

#include <Scene.hpp>

namespace gle {

  //! Handler for a skeleton
  /*!
    A skeleton is a group of Bone
  */
  class Skeleton : public Scene::Node {
  public:

    //! Create a skeleton
    Skeleton();

    //! Destroy skeleton
    ~Skeleton();

    //! Set skeleton ID
    /*!
      \param id ID to set
    */
    void	setId(GLint id);

    //! Return skeleton ID
    GLint	getId() const;

  private:
    GLint	_id;
  };

}
#endif
