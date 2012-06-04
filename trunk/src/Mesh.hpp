//
// Mesh.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Mon Feb 20 13:20:54 2012 loick michard
// Last update Wed May 30 17:41:45 2012 gael jochaud-du-plessix
//

#ifndef _MESH_HPP_
# define _MESH_HPP_

# include <string>
# include <vector>
# include <gle/opengl.h>
# include <Buffer.hpp>
# include <Matrix4.hpp>
# include <Matrix3.hpp>
# include <Vector3.hpp>
# include <Material.hpp>
# include <Quaternion.hpp>
# include <Array.hpp>
# include <MeshBufferManager.hpp>
# include <BoundingVolume.hpp>
# include <Octree.hpp>
# include <Scene.hpp>

namespace gle {
  
  //! Meshes class
  /*!
    This class creates, initializes and operates on meshes.
  */

  class Mesh : public Octree::Element, public Scene::Node {
  public:

    //! Different types of primitive that can be drawn

    enum PrimitiveType {
      //! Represent triangles defined by series of 3 vertices
      Triangles = GL_TRIANGLES,
      //! Represent lines defined by successing vertices
      Lines = GL_LINES,
      // Represent points defined by vertices
      Points = GL_POINTS
    };

    //! Ways of rasterizing the mesh

    enum RasterizationMode {
      //! Fill the primitives
      Fill = GL_FILL,
      //! Just draw outlines of the primitives
      Line = GL_LINE,
      // Just draw edges (vertices) of the primitives
      Point = GL_POINT
    };

    //! Size of the vertex coords  attribute
    static const GLsizeiptr VertexAttributeSizeCoords = 3;

    //! Size of the vertex normal  attribute
    static const GLsizeiptr VertexAttributeSizeNormal = 3;

    //! Size of the vertex texture coords  attribute
    static const GLsizeiptr VertexAttributeSizeTextureCoords = 2;

    //! Size of a vertex packed attributes

    static const GLsizeiptr VertexAttributesSize =
      (VertexAttributeSizeCoords
       + VertexAttributeSizeNormal
       + VertexAttributeSizeTextureCoords);

    //! Default constructor

    Mesh(Material* material=NULL);

    //! Copy constructor

    Mesh(Mesh const & other);
    
    //! Default destructor

    ~Mesh();

    //! Set the primitive type of the mesh

    void setPrimitiveType(PrimitiveType type);

    //! Get the primitive type of the mesh

    PrimitiveType getPrimitiveType() const;

    //! Set the rasterization mode of the mesh

    void setRasterizationMode(RasterizationMode rasterizationMode);

    //! Get the rasterization mode of the mesh

    RasterizationMode getRasterizationMode() const;

    //! Set the point size for mesh with point rasterization mode

    void setPointSize(GLfloat pointSize);

    //! Get the point size for mesh with point rasterization mode

    GLfloat getPointSize() const;

    //! Set the mesh vertex attributes

    void setVertexAttributes(const GLfloat* attributes, GLsizeiptr nbVertexes);

    //! Set the mesh vertexes

    void setVertexes(const GLfloat* vertexes, GLsizeiptr size, bool boundingBox = true);

    //! Set the mesh normals

    void setNormals(const GLfloat* normals, GLsizeiptr size);

    //! Set the mesh texture coords

    void setTextureCoords(const GLfloat* textureCoords, GLsizeiptr size);

    //! Set the mesh indexes

    void setIndexes(const GLuint* indexes, GLsizeiptr size);

    //! Set the mesh vertexes

    void setVertexes(gle::Array<GLfloat> const &vertexes, bool boundingBox = true);

    //! Set the mesh normals

    void setNormals(gle::Array<GLfloat> const &vertexes);

    //! Set the mesh texture coords

    void setTextureCoords(gle::Array<GLfloat> const &textureCoords);

    //! Set the mesh indexes

    void setIndexes(gle::Array<GLuint> const &vertexes);

    //! Set the mesh material

    void setMaterial(Material* material);

    //! Get the mesh material

    Material* getMaterial() const;

    //! Get the vertexes buffer

    Buffer<GLfloat> * getVertexesBuffer();

    //! Get the normals buffer

    Buffer<GLfloat> * getNormalsBuffer();

    //! Return the Texture coords buffer

    Buffer<GLfloat> * getTextureCoordsBuffer();

    //! Get the indexes buffer

    Buffer<GLuint> * getIndexesBuffer() const;
    
    //! Get the number of indexes in the mesh

    GLsizeiptr getNbIndexes() const;

    //! Get the number of vertexes in the mesh

    GLsizeiptr getNbVertexes() const;

    //! Get the attributes chunk in the mesh buffer manager

    gle::MeshBufferManager::Chunk* getAttributes() const;

    //! Get the bounding volume of the mesh

    BoundingVolume* getBoundingVolume() const;

    //! Get the coords of the highest values of the mesh vertices

    virtual const Vector3<GLfloat>& getMaxPoint() const;

    //! Get the coords of the lowest values of the mesh vertices

    virtual const Vector3<GLfloat>& getMinPoint() const;

    //! Get the coords of the point representing the center of the mesh

    virtual const Vector3<GLfloat>& getCenter() const;

    //! Return wether the mesh is in a frutum or not

    virtual bool isInFrustum(const GLfloat frustum[6][4]) const;

    //! Duplicate the mesh

    virtual Node* duplicate() const;

    //! Update the mesh

    virtual void update();
    
  private:
    PrimitiveType	_primitiveType;
    RasterizationMode	_rasterizationMode;
    GLfloat	_pointSize;

    Material* _material;
    Buffer<GLuint>* _indexes;
    MeshBufferManager::Chunk*	_attributes;

    GLsizeiptr		_nbIndexes;
    GLsizeiptr		_nbVertexes;
    BoundingVolume*	_boundingVolume;
  };
}

#endif // _MESH_HPP_
