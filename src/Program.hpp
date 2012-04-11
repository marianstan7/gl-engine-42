//
// Program.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Feb 17 16:21:24 2012 gael jochaud-du-plessix
// Last update Thu Apr 12 00:11:06 2012 loick michard
//

#ifndef _GLE_PROGRAM_HPP_
# define _GLE_PROGRAM_HPP_

# include <string>
# include <vector>
# include <Shader.hpp>
# include <Matrix4.hpp>
# include <Matrix3.hpp>
# include <Color.hpp>

namespace gle {

  //! Class representing an OpenGL Program
  /*!
    Encapsulation for creating a OpenGL Program, attaching Shaders to it,
    linking it and use it.
   */

  class Program {
  public:

    //! Uniforms that can be set in a program
    enum Uniform {
      MVMatrix,
      PMatrix,
      ColorMap,
      HasColorMap,
      NMatrix,
      AmbientColor,
      DiffuseColor,
      SpecularColor,
      DirectionalLightDirection,
      DirectionalLightColor,
      PointLightPosition,
      PointLightColor,
      PointLightSpecularColor,
      Shininess,
      SpecularIntensity,
      DiffuseIntensity
    };

    enum TextureUnit {
      ColorMapTexture = GL_TEXTURE0,
      ColorMapTextureIndex = 0,
    };
    
    //! Create a new OpenGL Program
    /*!
      Throw an OpenGLError if the program cannot be created
     */
    Program();

    //! Destruct a program
    ~Program();

    //! Attach a shader to a program
    /*!
      Throw an InvalidOperation if the shader is already attached to the program
      \param shader The shader to attach to the program
     */
    void attach(Shader const & shader);

    //! Link the program
    /*!
      If an error occur, throw a LinkageError with the details of the error
    */
    void link();

    //! Return the program the info log
    /*!
      Get the shader info log and returns it as a std::string.
      The info log can contain informations on linkage errors or warnings.
     */
    std::string getInfoLog() const;

    //! Use this program in the current OpenGL context
    /*!
      Throw an InvalidOperation if the program cannot be used
    */
    void use() const;

    //! Return the location of a uniform variable from its name
    GLint getUniformLocation(const GLchar *name) const;

    //! Return the location of a uniform variable
    GLint getUniformLocation(Uniform uniform);

    //! Return the saved location of a uniform variable
    GLint getSavedUniformLocation(Uniform uniform) const;

    //! Set the value of a uniform matrix
    void setUniform(Uniform uniformLocation, Matrix4<GLfloat> & matrix);

    //! Set the value of a uniform matrix3
    void setUniform(Uniform uniformLocation, Matrix3<GLfloat> & matrix);

    //! Set the value of a uniform array of vec3
    void setUniform3v(Uniform uniformLocation, GLfloat* values,
		      GLsizeiptr size);

    void setUniform(Uniform uniform, gle::Color<GLfloat> const & color);

    //! Set a value of an uniform GLfloat
    void setUniform(Uniform uniform, GLfloat value);

    //! Set a value of an uniform texture
    void setUniform(Uniform uniform, TextureUnit texture);

    //! Set a value of an uniform bool
    void setUniform(Uniform uniform, bool value);

    //! Return the OpenGL Program identifier
    GLuint getId() const;

  private:
    GLuint _id;

    std::vector<GLint> _uniformLocations;
    std::vector<std::string> _uniformNames;
  };

}

#endif /* _GLE_PROGRAM_HPP_ */
