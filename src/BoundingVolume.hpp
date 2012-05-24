//
// BoundingVolume.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Wed May  2 13:48:38 2012 loick michard
// Last update Mon May  7 15:51:29 2012 loick michard
//

#ifndef _BOUNDING_VOLUME_HPP_
# define _BOUNDING_VOLUME_HPP_

# include <gle/opengl.h>
# include <Octree.hpp>

namespace gle {

  class BoundingVolume : Octree::Element {
  public:
    virtual ~BoundingVolume(){};
    virtual BoundingVolume* duplicate() const = 0;
    virtual void setBestFit(const GLfloat* vertexes, GLsizeiptr size) = 0;
    virtual class Mesh* getMesh() const = 0;
    virtual void update(class Mesh* mesh) = 0;

    virtual const gle::Vector3<float>& getMaxPoint() const = 0;
    virtual const gle::Vector3<float>& getMinPoint() const = 0;
    virtual const gle::Vector3<float>& getCenter() const = 0;
    virtual bool isInFrustum(const GLfloat frustum[6][4]) const = 0;
  };
}

#endif
