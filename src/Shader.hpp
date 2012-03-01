//
// Shader.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Feb 17 13:32:25 2012 gael jochaud-du-plessix
// Last update Mon Feb 20 20:50:52 2012 gael jochaud-du-plessix
//

#ifndef _GLE_SHADER_HPP_
# define _GLE_SHADER_HPP_

# include <string>
# include <gle/opengl.h>

namespace gle {
  
  //! Class representing an OpenGL %Shader
  /*!
    Encapsulation of the creation, compilation and destruction of an
    OpenGL Shader.
   */

  class Shader {
  public:
    //! Shader types
    /*!
      Describes the different kind of shaders, for each programmable stage
      of the rendering pipeline
    */

    enum Type {
      Vertex = GL_VERTEX_SHADER,
      TesseleationControl = GL_TESS_CONTROL_SHADER,
      TesseleationEvaluation = GL_TESS_EVALUATION_SHADER,
      Geometry = GL_GEOMETRY_SHADER,
      Fragment = GL_FRAGMENT_SHADER
    };

    //! Construct a shader
    /*
      Create a new shader of type 'type'.
      If 'source' is specified and is not an empty string, it's assigned to the
      shader and is compiled.
      If the compilation fail, a CompilationError is thrown with the details
      of the error.
      \param type The shader type
      \param source The source of the shader
     */

    Shader(Type type, std::string source="");

    //! Destroy a shader
    ~Shader();

    //! Return the OpenGL shader identifier
    GLuint getId() const;

    //! Return the type of the shader
    Type getType() const;

    //! Set the source of the shader
    /*
      \param source A string representing the source of a GLSL shader of the
      current shader type
    */
    void setSource(std::string source);
    
    //! Compile the shader
    /*!
      Compile the shader.
      If the compilation fail, a CompilationError is thrown with the details
      of the error.
     */

    void compile();

    //! Return the shader info log
    /*!
      Get the shader info log and returns it as a std::string.
      The info log can contain informations on compilation errors or warnings.
     */

    std::string getInfoLog() const;

  private:
    Type _type;
    
    GLuint _id;
  };

}

#endif /* _GLE_SHADER_HPP_ */
