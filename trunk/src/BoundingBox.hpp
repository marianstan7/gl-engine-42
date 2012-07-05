//
// BoundingBox.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Wed May  2 16:06:37 2012 loick michard
// Last update Thu Jul  5 22:52:57 2012 gael jochaud-du-plessix
//

#ifndef _BOUNDING_BOX_HPP
# define _BOUNDING_BOX_HPP

# include <BoundingVolume.hpp>
# include <Mesh.hpp>
# include <Material.hpp>

namespace gle
{
  //! Bounding Box for meshes and frustum culling
  class BoundingBox : public BoundingVolume {
  public:
    //! Create a bounding box
    BoundingBox();

    //! Destroy bounding box
    virtual ~BoundingBox();
    
    //! Duplicate a bounding box
    virtual BoundingVolume* duplicate() const;

    //! Compute the best fit bounding boxs for a mesh
    /*!
      \param vertexes Tab of all mesh vertexes
      \param size Number of vertexes
     */
    virtual void setBestFit(const GLfloat* vertexes, GLsizeiptr size);

    //! Compute the best fit bounding boxs for a mesh
    /*!
      \param datas Tab of mesh datas
      \param offset Start offset of vertexes in datas
      \param attributeSize Size of vertex attribute in datas
      \param nbVertexes Number of vertexes
     */
    virtual void setBestFit(const GLfloat* datas, GLsizeiptr offset, GLsizeiptr attributeSize, GLsizeiptr nbVertexes);

    //! Return the bounding box debug mesh
    /*!
      This is used only if renderer debug mode is activated and set to Renderer::BoundingBox
      /param mesh Linked mesh
    */
    virtual Mesh* getDebugMesh(Mesh* mesh);

    virtual void update(const Matrix4<GLfloat>& mvMatrix);

    //! Get the highest extreme value in the 3 axis
    virtual const gle::Vector3<float>& getMaxPoint() const;

    //! Get the lowest extreme value in the 3 axis
    virtual const gle::Vector3<float>& getMinPoint() const; 

    //! Get center of bounding box
    virtual const gle::Vector3<float>& getCenter() const; 

    //! Return wheter or not bounding box is in frustum
    /*!
      \param frustum Six planes of frustum
     */
    virtual bool isInFrustum(const GLfloat frustum[6][4]) const;
    
  private:
    Vector3<GLfloat> _points[8];
    Vector3<GLfloat> _absolutePoints[8];
    Vector3<GLfloat> _center;
    Vector3<GLfloat> _absoluteCenter;
    Material*	_debugMaterial;
    Mesh*	_debugMesh;
    Vector3<GLfloat> _min;
    Vector3<GLfloat> _max;
  };
}

#endif
