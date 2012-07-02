//
// Mesh.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Jun 21 20:39:52 2012 loick michard
// Last update Mon Jul  2 17:24:01 2012 gael jochaud-du-plessix
//

#ifndef _MESH_HPP_
# define _MESH_HPP_

# include <string>
# include <vector>
# include <list>
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
# include <Texture.hpp>
# include <EnvironmentMap.hpp>

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

    //! Size of the vertex coords attribute
    static const GLsizeiptr VertexAttributeSizeCoords = 3;

    //! Size of the vertex normal attribute
    static const GLsizeiptr VertexAttributeSizeNormal = 3;

    //! Size of the vertex tangent attribute
    static const GLsizeiptr VertexAttributeSizeTangent = 3;

    //! Size of the vertex texture coords attribute
    static const GLsizeiptr VertexAttributeSizeTextureCoords = 2;

    //! Size of the vertex bones infos attribute

    static const GLsizeiptr VertexAttributeSizeBone = 4;

    //! Size of the vertex mesh identifiers attribute
    static const GLsizeiptr VertexAttributeMeshIdentifiers = 3;

    //! Size of a vertex packed attributes

    static const GLsizeiptr VertexAttributesSize = 
      (VertexAttributeSizeCoords
       + VertexAttributeSizeNormal
       + VertexAttributeSizeTangent
       + VertexAttributeSizeTextureCoords
       + VertexAttributeSizeBone
       + VertexAttributeMeshIdentifiers)
      ;

    //! Size of the datas used by one mesh in the uniform buffer                                                                                             
    static const GLsizeiptr UniformSize = 20;


    //! Symbolize a group of meshes for rendering

    struct Group {
      std::list<Mesh*>		meshes;
      gle::Texture*		colorMap;
      gle::Texture*		normalMap;
      gle::EnvironmentMap*	envMap;
    };

    //! Factorize a list of meshes using canBeRenderedWith comparator

    static std::list<gle::Scene::MeshGroup> factorizeForDrawing(std::list<gle::Mesh*> meshes,
								bool ignoreBufferId=false,
								bool ignoreMaterial=false);

    //! Default constructor

    Mesh(Material* material=NULL, bool isDynamic=false);

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

    void setTangents(const GLfloat* tangents, GLsizeiptr size);

    //! Set the mesh texture coords

    void setTextureCoords(const GLfloat* textureCoords, GLsizeiptr size);

    //! Set the mesh indexes

    void setIndexes(const GLuint* indexes, GLsizeiptr size);

    //! Set the mesh vertexes

    void setVertexes(gle::Array<GLfloat> const &vertexes, bool boundingBox = true);

    //! Set the mesh normals

    void setNormals(gle::Array<GLfloat> const &vertexes);

    void setTangents(gle::Array<GLfloat> const &tangents);

    //! Set the mesh texture coords

    void setTextureCoords(gle::Array<GLfloat> const &textureCoords);

    //! Set the mesh indexes

    void setIndexes(gle::Array<GLuint> const &vertexes);

    //! Set the mesh identifiers

    void setIdentifiers(GLuint meshId, GLuint materialId);

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

    void createBoundingVolume(const GLfloat* datas, GLsizeiptr offset, GLsizeiptr attributeSize, GLsizeiptr nbVertexes);

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

    virtual void update();

    virtual void setDynamic(bool dynamic, bool deep=true);

    void makeAbsoluteIndexes(bool absolute=true);

    bool canBeRenderedWith(const gle::Scene::MeshGroup& other,
			   bool ignoreBufferId=false, bool ignoreMaterial=false) const;

    void setUniformBufferId(GLint index);

    void setMaterialBufferId(GLint index);

    GLint getUniformBufferId() const;

    GLint getMaterialBufferId() const;

    virtual std::vector<Scene::Node*>& getDebugNodes(int mode);

    virtual void      updateMatrix();

    const GLfloat* getUniforms();
    
  private:
    PrimitiveType	_primitiveType;
    RasterizationMode	_rasterizationMode;
    GLfloat		_pointSize;

    Material*			_material;
    Buffer<GLuint>*		_indexes;
    MeshBufferManager::Chunk*	_attributes;

    GLsizeiptr		_nbIndexes;
    GLsizeiptr		_nbVertexes;
    BoundingVolume*	_boundingVolume;

    GLint		_uniformBufferId;
    GLint		_materialBufferId;

    bool		_absoluteIndexes;

    bool		_needUniformsUpdate;
    GLfloat*		_uniforms;
  };
}

#endif // _MESH_HPP_
