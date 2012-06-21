//
// Buffer.hpp for  in /home/jochau_g//dev/opengl/glEngine
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 15 16:10:11 2012 gael jochaud-du-plessix
// Last update Thu Jun 21 00:11:08 2012 loick michard
//

#ifndef _GLE_BUFFER_HPP_
# define _GLE_BUFFER_HPP_

# include <gle/opengl.h>
# include <Exception.hpp>

#include <iostream>

namespace gle {
  
  //! Class for manipulating OpenGL Buffers
  /*!
    This class encapsulate the creation, initialisation, copy and destruction of
    an OpenGL Buffer.
    It also allows to map a buffer in memory or update part or all of its
    data once created.
  */

  template <typename T>
  class Buffer {
  public:
    //! Buffer types
    /*!
      Define the different possible types for a buffer
    */

    enum Type {
      VertexArray = GL_ARRAY_BUFFER,
      /*! Vertex array buffer, to store an array of vertex attributes */
      ElementArray = GL_ELEMENT_ARRAY_BUFFER,
      /*! Element array buffer, to store an array of vertex indices */
      UniformArray = GL_UNIFORM_BUFFER
      /*! Uniform buffer, to store program uniforms */
    };

    //! Buffer usages
    /*!
      Define the different kinds of usage for a buffer.
      Usage is provide as a performance hint for opengl, it does not constraint
      the actual usage of the buffer.
    */

    enum Usage {
      StaticDraw = GL_STATIC_DRAW
      /*!< Usage for a buffer modified by the application and used by OpenGL
	for drawing */
    };

    //! Mapping access types
    /*!
      Define the different types of access that can be used during the mapping
      of a buffer.
    */

    enum MapAccess {
      ReadOnly = GL_READ_ONLY,
      /*!< Map the buffer for read only use */
      WriteOnly = GL_WRITE_ONLY,
      /*!< Map the buffer for write only use */
      ReadWrite = GL_READ_WRITE
      /*!< Map the buffer for reading and writing */
    };

    //! Create a buffer
    /*!
      Size, data and usage are can be ommited.
      If size is specified and size > 0, the buffer data is initialized.
      If data is not NULL, the buffer is initialized with memory it points to.
      Otherwise, buffer values will be undefined.
      \param type Buffer type
      \param usage Buffer usage type
      \param size Size of the data
      \param data Buffer data
    */

    Buffer(Type type, Usage usage=StaticDraw,
	   GLsizeiptr size=0, const T* data=NULL) :
      _type(type), _usage(usage), _size(size), _id(0)
    {
      glGenBuffers(1, &_id);
      if (size > 0)
	{
	  bind();
	  glBufferData(_type, _size * sizeof(T), data, _usage);
	  GLenum error = glGetError();
	  if (error == GL_OUT_OF_MEMORY)
	    throw new gle::Exception::OutOfMemory("Cannot create buffer");
	  else if (error != GL_NO_ERROR)
	    throw new gle::Exception::OpenGLError();
	}
    }

    //! Copy a buffer
    /*!
      Create a new buffer with the content and properties of an existing one.
      Data are copied directly from gpu to gpu.
    */

    Buffer(Buffer const & other) :
      _type(other._type), _usage(other._usage), _size(0), _id(0)
    {
      glGenBuffers(1, &_id);
      if (other._size > 0)
	{
	  _size = other._size;
	  glBindBuffer(GL_COPY_WRITE_BUFFER, _id);
	  glBufferData(GL_COPY_WRITE_BUFFER, other._size * sizeof(T),
		       NULL, _usage);
	  glBindBuffer(GL_COPY_READ_BUFFER, other._id);
	  bind();
	  glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
			      0, 0, other._size * sizeof(T));
	  GLenum error = glGetError();
	  if (error == GL_OUT_OF_MEMORY)
	    throw new gle::Exception::OutOfMemory("Cannot copy buffer");
	  else if (error == GL_INVALID_VALUE)
	    throw new gle::Exception::InvalidValue();
	  else if (error == GL_INVALID_OPERATION)
	    throw new gle::Exception::InvalidOperation();
	  else if (error != GL_NO_ERROR)
	    throw new gle::Exception::OpenGLError();
	}
    }

    //! Copy a buffer with a new size
    /*!
      Create a new buffer with the content and properties of an existing one,
      and with the size specified.
      Only newSize bytes are copied from the existing buffer.
      Data are copied directly from gpu to gpu.
    */

    Buffer(Buffer const & other, GLsizeiptr newSize) :
      _type(other._type), _usage(other._usage), _size(newSize), _id(0)
    {
      glGenBuffers(1, &_id);
      if (_size < other._size)
	_size = other._size;
      glBindBuffer(GL_COPY_WRITE_BUFFER, _id);
      glBufferData(GL_COPY_WRITE_BUFFER, _size * sizeof(T),
		   NULL, _usage);
      glBindBuffer(GL_COPY_READ_BUFFER, other._id);
      bind();
      glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
			  0, 0, other._size * sizeof(T));
      GLenum error = glGetError();
      if (error == GL_OUT_OF_MEMORY)
	throw new gle::Exception::OutOfMemory("Cannot copy buffer");
      else if (error == GL_INVALID_VALUE)
	throw new gle::Exception::InvalidValue();
      else if (error == GL_INVALID_OPERATION)
	throw new gle::Exception::InvalidOperation();
      else if (error != GL_NO_ERROR)
	throw new gle::Exception::OpenGLError();
    }

    //! Destruct a buffer
    ~Buffer()
    {
      glDeleteBuffers(1, &_id);
    }

    //! Bind a buffer
    /*!
      Binding a buffer make it the 'current buffer' for all OpenGL operations
      on its Type of buffer.
    */

    void bind() const
    {
      glBindBuffer(_type, _id);
    }

    //! Bind a buffer to a binding point
    /*!
      Binding a buffer make it the 'current buffer' for all OpenGL operations
      on its Type of buffer.
    */

    void bindBase(GLuint binding) const
    {
      glBindBufferBase(_type, binding, _id);
    }

    //! Resize a buffer
    /*!
      Resize a buffer data and reinitialize its values.
      If 'data' is NULL, the new values of the buffer will be undefined.
      \param size The new buffer size
      \param data Pointer to the new datas
      \sa setData
    */

    void resize(GLsizeiptr size, const T* data=NULL)
    {
      if (size == _size && !data)
	return ;
      _size = size;
      setData(data);
    }

    //! Set the buffer data
    /*!
      Update the buffer data with values pointed by 'data'
      \param data Must point to a valid memory space which size is at least
      the buffer's size.
      \sa resize
    */

    void setData(const T* data)
    {
      bind();
      GLenum error = glGetError();
      if (error != GL_NO_ERROR)
	{
	  throw new gle::Exception::OpenGLError("setData: bind()");
	}
      glBufferData(_type, _size * sizeof(T), data, _usage);
      error = glGetError();
      if (error == GL_OUT_OF_MEMORY)
	throw new gle::Exception::OutOfMemory("Cannot set buffer data");
      else if (error != GL_NO_ERROR)
	{
	  throw new gle::Exception::OpenGLError("setData: glBufferData()");
	}
    }

    //! Set a part of the buffer data
    /*!
      Update the data range defined by 'offset' and 'offset + size'
      with values pointed by 'data'
      \param data Must point to a valid memory space which size is at least
      'size'
      \param offset Start of the range to set
      \param size Size of the range to set
      \sa resize
    */

    void setData(const T* data, GLintptr offset, GLsizeiptr size)
    {
      bind();
      glBufferSubData(_type, offset * sizeof(T), size * sizeof(T), data);
      GLenum error = glGetError();
      if (error == GL_INVALID_VALUE)
	throw new gle::Exception::InvalidValue("Invalid offset or size");
      else if (error == GL_INVALID_OPERATION)
	throw new gle::Exception::InvalidOperation("Buffer is mapped");
      else if (error != GL_NO_ERROR)
	throw new gle::Exception::OpenGLError();
    }

    //! Map a buffer in memory
    /*!
      Mapping a buffer allows the application to directly access the buffer's
      datas.
      \param access Specifies the type of access required for this mapping
      \return A pointer to the mapped space
      \sa unmap
    */

    T* map(MapAccess access=ReadWrite)
    {
      bind();
      T* ptr = (T*)glMapBuffer(_type, access);
      GLenum error = glGetError();
      if (error == GL_OUT_OF_MEMORY)
	throw new gle::Exception::OutOfMemory("Cannot map buffer");
      else if (error == GL_INVALID_OPERATION)
	throw new gle::Exception::InvalidOperation("Buffer already mapped");
      else if (error != GL_NO_ERROR || ptr == NULL)
	throw new gle::Exception::OpenGLError();
      return (ptr);
    }

    //! Map part of a buffer in memory
    /*!
      Mapping a buffer allows the application to directly access the buffer's
      datas.
      This functions map only the part of the buffer, starting at 'offset'
      and of size 'length'.
      \param offset The offset of the start of the part to be mapped
      \param length Length of the part to be mapped
      \param access Specifies the type of access required for this mapping
      \return A pointer to the mapped space
      \sa unmap
    */

    T* map(GLintptr offset, GLsizeiptr length, MapAccess access=ReadWrite)
    {
      bind();
      GLbitfield accessBits = 0;
      if (access == ReadOnly)
	accessBits = GL_MAP_READ_BIT;
      else if (access == WriteOnly)
	accessBits = GL_MAP_WRITE_BIT;
      else if (access == ReadWrite)
	accessBits = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
      T* ptr = (T*)glMapBufferRange(_type, offset * sizeof(T),
				    length * sizeof(T), accessBits);
      GLenum error = glGetError();
      if (error == GL_INVALID_VALUE)
	throw new gle::Exception::InvalidValue("Invalid offset or length");
      else if (error == GL_OUT_OF_MEMORY)
	throw new gle::Exception::OutOfMemory("Cannot map buffer");
      else if (error == GL_INVALID_OPERATION)
	throw new gle::Exception::InvalidOperation();
      else if (error != GL_NO_ERROR || ptr == NULL)
	throw new gle::Exception::OpenGLError();
      return (ptr);
    }

    //! Unmap the buffer
    /*!
      When unmap is called, all pointer previously returned by a map call
      become invalid.\n
      unmap returns true if there is no error.\n
      If the buffer data has been corrupted during the period the buffer was
      mapped (this can appen for rare system reasons), unmap returns false and
      the buffer data must be reinitialized.
      \return True unless the buffer data has been corrupted
      \sa map
    */

    bool unmap()
    {
      bind();
      GLboolean ret = glUnmapBuffer(_type);
      GLenum error = glGetError();
      if (error == GL_INVALID_OPERATION)
	throw new gle::Exception::InvalidOperation("Buffer not mapped");
      else if (error != GL_NO_ERROR)
	throw new gle::Exception::OpenGLError();
      return (ret);
    }

    //! Flush a range of a mapped buffer
    /*!
      Flush indicates modifications to a range of a mapped buffer
      \param offset Begining of the range
      \param length Length of the range
     */
    
    void flush(GLintptr offset, GLsizeiptr length)
    {
      bind();
      glFlushMappedBuffer(_type, offset, length);
      GLenum error = glGetError();
      if (error == GL_INVALID_VALUE)
	throw new gle::Exception::InvalidValue("Flush invalid");
      else if (error == GL_INVALID_OPERATION)
	throw new gle::Exception::InvalidOperation("Cannot flush buffer");
      else if (error != GL_NO_ERROR)
	throw new gle::Exception::OpenGLError();
    }

    //! Return the buffer type
    Type	getType() const
    {
      return (_type);
    }

    //! Return the buffer kind of usage
    Usage	getUsage() const
    {
      return (_usage);
    }

    //! Return the buffer size
    GLsizeiptr	getSize() const
    {
      return (_size);
    }

    //! Return the buffer OpenGL object name
    GLuint	getId() const
    {
      return (_id);
    }
    
  private:
    Type	_type;
    Usage	_usage;
    GLsizeiptr	_size;

    GLuint	_id;
  };

  typedef Buffer<GLfloat> Bufferf;
  typedef Buffer<GLuint> Bufferui;
};

#endif /* _GLE_BUFFER_HPP_ */
