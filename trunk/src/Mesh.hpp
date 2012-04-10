//
// Mesh.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Mon Feb 20 13:20:54 2012 loick michard
// Last update Fri Apr  6 23:27:18 2012 gael jochaud-du-plessix
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

namespace gle {
  
  //! Meshes class
  /*!
    This class creates, initializes and operates on meshes.
  */

  class Mesh {
  public:

    enum PrimitiveType {
      Triangles = GL_TRIANGLES,
      Lines = GL_LINES,
      Points = GL_POINTS
    };

    enum RasterizationMode {
      Fill = GL_FILL,
      Line = GL_LINE,
      Point = GL_POINT
    };
    
    //! Default constructor
    
    Mesh(Material* material=NULL,
	 const GLfloat* vertexes=NULL, GLsizeiptr nbVertexes=0,
	 const GLfloat* normals=NULL, GLsizeiptr nbNormals=0,
	 const GLuint* indexes=NULL, GLsizeiptr nbIndexes=0);

    //! Constructor with attribute arrays

    Mesh(Material* material,
	 gle::Array<GLfloat> const * vertexes=NULL,
	 gle::Array<GLfloat> const * normals=NULL,
	 gle::Array<GLuint> const * indexes=NULL);

    //! Copy constructor
    Mesh(Mesh const & other);
    
    //! Default destructor

    ~Mesh();

    //! Add a child to mesh
    /*
      \param child Pointer to the child to add
    */

    void addChild(Mesh* child);

    //! Get the mesh children list
    /*!
      Return the std::vector of children, allowing to directly
      loop through them.
     */

    std::vector<Mesh*> & getChildren();

    //! Recursively search for children matching a given name
    
    void getChildrenByName(std::string const & name,
			   std::vector<gle::Mesh*> & vector);

    //! Search for a child matching a given name

    gle::Mesh* getChildByName(std::string const & name);

    //! Set the name of the mesh
    /*!
      param name New name
     */

    void setName(std::string const & name);

    //! Return the name of the mesh

    std::string const & getName();

    void setType(PrimitiveType type);

    gle::Mesh::PrimitiveType getType() const;

    void setRasterizationMode(RasterizationMode rasterizationMode);

    gle::Mesh::RasterizationMode getRasterizationMode() const;

    void setPointSize(GLfloat pointSize);

    GLfloat getPointSize();

    //! Set the movement matrix of parent
    /*!
      \param parentMatrix Pointer the the parent matrix
    */

    void setParentMatrix(Matrix4<GLfloat>* parentMatrix=NULL);

    //! Update the movement matrix

    void updateMatrix();

    //! Translate a mesh
    /*!
      \param vec Vector of translation
    */
    
    void translate(Vector3<GLfloat> const& vec);

    //! Set the position of the mesh

    void setPosition(Vector3<GLfloat> const& pos);

    //! Set the rotation of the mesh
    /*!
      \param axis Axis of the rotation
      \param angle Angle of the rotation
     */

    void setRotation(Vector3<GLfloat> const& axis, GLfloat angle);

    //! Set the rotation target
    /*!
      \param target The point the mesh should 'look at'
     */
    void setRotationTarget(const Vector3<GLfloat>& target);

    //! Set scale
    /*!
      \param x Scale coefficient on x
      \param y Scale coefficient on y
      \param z Scale coefficient on z
    */
    
    void setScale(GLfloat x, GLfloat y, GLfloat z);

    //! Set scale
    /*!
      Is equivalent of setScale(scale, scale, scale)
      \param scale Scale coefficient
    */

    void setScale(GLfloat scale);

    //! Set the mesh vertexes

    void setVertexes(const GLfloat* vertexes, GLsizeiptr nbVertexes);

    //! Set the mesh normals

    void setNormals(const GLfloat* normals, GLsizeiptr nbNormals);

    //! Set the mesh texture coords

    void setTextureCoords(const GLfloat* textureCoords, GLsizeiptr size);

    //! Set the mesh indexes

    void setIndexes(const GLuint* indexes, GLsizeiptr nbIndexes);

    //! Set the mesh vertexes

    void setVertexes(gle::Array<GLfloat> const &vertexes);

    //! Set the mesh normals

    void setNormals(gle::Array<GLfloat> const &vertexes);

    //! Set the mesh texture coords

    void setTextureCoords(gle::Array<GLfloat> const &textureCoords);

    //! Set the mesh indexes

    void setIndexes(gle::Array<GLuint> const &vertexes);

    //! Return movement matrix

    Matrix4<GLfloat>& getMatrix();

    //! Set the mesh material

    void setMaterial(Material* material);
    
    //! Get the normal 3x3 matrix of the mesh
    /*!
      The normal matrix is the transposed inverse of the 4x4
      model matrix. It's used to transform normals without
      altering it.
     */

    Matrix3<GLfloat>& getNormalMatrix();

    //! Get the mesh material

    Material* getMaterial();

    //! Get the vertexes buffer

    Buffer<GLfloat> * getVertexesBuffer();

    //! Get the normals buffer

    Buffer<GLfloat> * getNormalsBuffer();

    //! Return the Texture coords buffer

    Buffer<GLfloat> * getTextureCoordsBuffer();

    //! Get the indexes buffer

    Buffer<GLuint> * getIndexesBuffer();
    
    //! Get the number of indexes in the mesh

    GLsizeiptr getNbIndexes();

  private:
    std::string		_name;
    PrimitiveType	_type;
    RasterizationMode	_rasterizationMode;
    GLfloat	_pointSize;

    Material* _material;
    Buffer<GLfloat>* _vertexes;
    Buffer<GLfloat>* _normals;
    Buffer<GLfloat>* _textureCoords;
    Buffer<GLuint>* _indexes;

    GLsizeiptr _nbIndexes;

    Vector3<GLfloat> _position;
    Vector3<GLfloat> _target;
    bool _hasTarget;
    Quaternion<GLfloat> _rotation;
    Matrix4<GLfloat> _scaleMatrix;

    std::vector<Mesh*> _children;

    Matrix4<GLfloat> _mvMatrix;
    Matrix3<GLfloat> _normalMatrix;
    Matrix4<GLfloat>* _parentMatrix;
  };
}

#endif // _MESH_HPP_
