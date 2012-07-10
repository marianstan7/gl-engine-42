//
// BoundingVolume.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Wed May  2 13:48:38 2012 loick michard
// Last update Tue Jul 10 15:36:48 2012 loick michard
//

#ifndef _BOUNDING_VOLUME_HPP_
# define _BOUNDING_VOLUME_HPP_

# include <gle/opengl.h>
# include <Octree.hpp>

namespace gle {

  //! Bounding volumes for meshes and frustum culling
  class BoundingVolume : Octree::Element {
  public:
    //! Destroy bounding volume
    virtual ~BoundingVolume(){};

    //! Duplicate a bounding volume
    virtual BoundingVolume* duplicate() const = 0;

    //! Compute the best fit bounding volumes for a mesh
    /*!
      \param vertexes Tab of all mesh vertexes
      \param size Number of vertexes
     */
    virtual void setBestFit(const GLfloat* vertexes, GLsizeiptr size) = 0;

    //! Compute the best fit bounding volumes for a mesh
    /*!
      \param datas Tab of mesh datas
      \param offset Start offset of vertexes in datas
      \param attributeSize Size of vertex attribute in datas
      \param nbVertexes Number of vertexes
     */
    virtual void setBestFit(const GLfloat* datas, GLsizeiptr offset, GLsizeiptr attributeSize, GLsizeiptr nbVertexes) = 0;

    //! Return the bounding volume debug mesh
    /*!
      This is used only if renderer debug mode is activated and set to Renderer::BoundingVolume
      /param mesh Linked mesh
    */
    virtual class Mesh* getDebugMesh(class Mesh* mesh) = 0;

    //! Update bounding volume after mesh transformation
    /*!
      \param mvMatrix Mesh transformation matrix
     */
    virtual void update(const Matrix4<GLfloat>& mvMatrix) = 0;


    //! Get the highest extreme value in the 3 axis
    virtual const gle::Vector3<float>& getMaxPoint() = 0;

    //! Get the lowest extreme value in the 3 axis
    virtual const gle::Vector3<float>& getMinPoint() = 0;

    //! Get center of bounding volume
    virtual const gle::Vector3<float>& getCenter() = 0;

    //! Return wheter or not bounding volume is in frustum
    /*!
      \param frustum Six planes of frustum
     */
    virtual bool isInFrustum(const GLfloat frustum[6][4]) const = 0;
  };
}

#endif
