//
// BoundingSphere.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Wed May  2 16:06:37 2012 loick michard
// Last update Tue Jul 10 15:37:30 2012 loick michard
//

#ifndef _BOUNDING_SPHERE_HPP
# define _BOUNDING_SPHERE_HPP

# include <BoundingVolume.hpp>
# include <Mesh.hpp>
# include <Material.hpp>

namespace gle
{
  //! Bounding Sphere for meshes and frustum culling
  class BoundingSphere : public BoundingVolume {
  public:

    //! Create an empty bounding sphere
    BoundingSphere();

    //! Delete bounding sphere
    virtual ~BoundingSphere();
    
    //! Duplicate a bounding sphere
    virtual BoundingVolume* duplicate() const;

    //! Compute the best fit bounding spheres for a mesh
    /*!
      \param vertexes Tab of all mesh vertexes
      \param size Number of vertexes
     */
    virtual void setBestFit(const GLfloat* vertexes, GLsizeiptr size);

    //! Compute the best fit bounding spheres for a mesh
    /*!
      \param datas Tab of mesh datas
      \param offset Start offset of vertexes in datas
      \param attributeSize Size of vertex attribute in datas
      \param nbVertexes Number of vertexes
     */
    virtual void setBestFit(const GLfloat* datas, GLsizeiptr offset, GLsizeiptr attributeSize, GLsizeiptr nbVertexes);

    //! Return the bounding sphere debug mesh
    /*!
      This is used only if renderer debug mode is activated and set to Renderer::BoundingSphere
      /param mesh Linked mesh
    */
    virtual Mesh* getDebugMesh(Mesh* mesh);

    virtual void update(const Matrix4<GLfloat>& mvMatrix);

    //! Get the highest extreme value in the 3 axis
    virtual const gle::Vector3<float>& getMaxPoint();

    //! Get the lowest extreme value in the 3 axis
    virtual const gle::Vector3<float>& getMinPoint();

    //! Get center of bounding sphere
    virtual const gle::Vector3<float>& getCenter();

    //! Return wheter or not bounding sphere is in frustum
    /*!
      \param frustum Six planes of frustum
     */
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
