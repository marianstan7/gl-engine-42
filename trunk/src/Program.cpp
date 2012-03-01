//
// Program.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Feb 17 16:21:14 2012 gael jochaud-du-plessix
// Last update Thu Mar  1 18:29:24 2012 gael jochaud-du-plessix
//

#include <Program.hpp>
#include <Exception.hpp>

gle::Program::Program() :
  _id(0), _uniformLocations(32), _uniformNames(32)
{
  _uniformNames[gle::Program::MVMatrix] = "gle_MVMatrix";
  _uniformNames[gle::Program::PMatrix] = "gle_PMatrix";
  _uniformNames[gle::Program::color] = "gle_color";
  _uniformNames[gle::Program::NMatrix] = "gle_NMatrix";
  _uniformNames[gle::Program::ambientColor] = "gle_ambientColor";
  _uniformNames[gle::Program::directionalLightDirection] = "gle_directionalLightDirection";
  _uniformNames[gle::Program::directionalLightColor] = "gle_directionalLightColor";
  _uniformNames[gle::Program::pointLightPosition] = "gle_pointLightPosition";
  _uniformNames[gle::Program::pointLightColor] = "gle_pointLightColor";
  _uniformNames[gle::Program::pointLightSpecularColor] = "gle_pointLightSpecularColor";
  _uniformNames[gle::Program::shininess] = "gle_shininess";
  _uniformNames[gle::Program::diffuseIntensity] = "gle_diffuseIntensity";
  _uniformNames[gle::Program::specularIntensity] = "gle_specularIntensity";

  _id = glCreateProgram();
  if (_id == 0)
    throw new gle::Exception::OpenGLError("Cannot create program");
}

gle::Program::~Program()
{
  glDeleteProgram(_id);
}

void gle::Program::attach(Shader const & shader)
{
  glAttachShader(_id, shader.getId());
  GLenum error = glGetError();
  if (error == GL_INVALID_OPERATION)
    throw new gle::Exception::InvalidOperation("Trying to attach a shader already attached to a program");
  else if (error != GL_NO_ERROR)
    throw new gle::Exception::OpenGLError();
}

void gle::Program::link()
{
  GLint status;

  glLinkProgram(_id);
  GLenum error = glGetError();
  if (error == GL_INVALID_OPERATION)
    throw new gle::Exception::InvalidOperation("Cannot link the program");
  else if (error != GL_NO_ERROR)
    throw new gle::Exception::OpenGLError();
  glGetProgramiv(_id, GL_LINK_STATUS, &status);
  if (status != GL_TRUE)
    throw new gle::Exception::LinkageError(getInfoLog());
}

std::string gle::Program::getInfoLog() const
{
  GLint length;

  glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
  if (length <= 0)
    return ("");
  char *buffer = new char[length];
  glGetProgramInfoLog(_id, length, NULL, buffer);
  std::string infoLog(buffer);
  delete[] buffer;
  return (infoLog);
}

void gle::Program::use() const
{
  glUseProgram(_id);
  GLenum error = glGetError();
  if (error == GL_INVALID_OPERATION)
    throw new gle::Exception::InvalidOperation("Program cannot be used");
  else if (error != GL_NO_ERROR)
    throw new gle::Exception::OpenGLError();
}

GLint gle::Program::getUniformLocation(const GLchar *name) const
{
  GLint location = glGetUniformLocation(_id, name);
  if (location == -1)
    throw new gle::Exception::InvalidOperation("Uniform doesn't exists");
  else if (glGetError() != GL_NO_ERROR)
    throw new gle::Exception::OpenGLError();
  return (location);
}

GLint gle::Program::getUniformLocation(gle::Program::Uniform uniform)
{
  GLint location = getUniformLocation(_uniformNames[uniform].c_str()); 
  _uniformLocations[uniform] = location;
  return (location);
}

GLint gle::Program::getSavedUniformLocation(gle::Program::Uniform uniform) const
{
  return (_uniformLocations[uniform]);
}

GLuint gle::Program::getId() const
{
  return (_id);
}

void gle::Program::setUniform(Uniform uniform, Matrix4<GLfloat> & matrix)
{
  glUniformMatrix4fv(_uniformLocations[uniform], 1, GL_FALSE,
		     (GLfloat*)matrix);
}

void gle::Program::setUniform(Uniform uniform, Matrix3<GLfloat> & matrix)
{
  glUniformMatrix3fv(_uniformLocations[uniform], 1, GL_FALSE,
		     (GLfloat*)matrix);
}

void gle::Program::setUniform3v(Uniform uniform, GLfloat* values,
				GLsizeiptr size)
{
  glUniform3fv(_uniformLocations[uniform], size, values);
}

void gle::Program::setUniform(Uniform uniform, GLfloat value)
{
  glUniform1f(_uniformLocations[uniform], value);
}

void gle::Program::setUniform(Uniform uniform,
			      gle::Color<GLfloat> const & color)
{
  glUniform3f(_uniformLocations[uniform], color.r, color.g, color.b);
}
