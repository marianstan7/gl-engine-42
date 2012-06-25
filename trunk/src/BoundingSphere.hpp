//
// BoundingSphere.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Wed May  2 16:06:37 2012 loick michard
// Last update Sat Jun 23 14:53:35 2012 loick michard
//

#ifndef _BOUNDING_SPHERE_HPP
# define _BOUNDING_SPHERE_HPP

# include <BoundingVolume.hpp>
# include <Mesh.hpp>
# include <Material.hpp>

namespace gle
{
  class BoundingSphere : public BoundingVolume {
  public:
    BoundingSphere();
    virtual ~BoundingSphere();
    
    virtual BoundingVolume* duplicate() const;
    virtual void setBestFit(const GLfloat* vertexes, GLsizeiptr size);
    virtual void setBestFit(const GLfloat* datas, GLsizeiptr offset, GLsizeiptr attributeSize, GLsizeiptr nbVertexes);
    virtual Mesh* getDebugMesh(Mesh* mesh);
    virtual void update(Mesh* mesh);

    virtual const gle::Vector3<float>& getMaxPoint() const;
    virtual const gle::Vector3<float>& getMinPoint() const;
    virtual const gle::Vector3<float>& getCenter() const;
    virtual bool isInFrustum(const GLfloat frustum[6][4]) const;
    
  private:
    Vector3<GLfloat> _center;
    Vector3<GLfloat> _absoluteCenter;
    GLfloat	_radius;
    GLfloat	_absoluteRadius;
    Material*	_debugMaterial;
    Mesh*	_debugMesh;
    Vector3<GLfloat> _min;
    Vector3<GLfloat> _max;
  };
}

#endif
