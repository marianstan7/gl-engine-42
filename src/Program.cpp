//
// Program.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Feb 17 16:21:14 2012 gael jochaud-du-plessix
// Last update Mon Jul  2 22:11:24 2012 loick michard
//

#include <Program.hpp>
#include <Exception.hpp>

gle::Program::Program() :
  _id(0), _currentUniformBlockBinding(0)
{
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

GLint gle::Program::getUniformLocation(const GLchar *name)
{
  GLint location = glGetUniformLocation(_id, name);
  _uniformLocations[name] = location;
  if (location == -1)
    throw new gle::Exception::InvalidOperation(std::string(name) + ": Uniform doesn't exists");
  else if (glGetError() != GL_NO_ERROR)
    throw new gle::Exception::OpenGLError();
  return (location);
}

GLint gle::Program::getSavedUniformLocation(const GLchar *name)
{
  return (_uniformLocations[name]);
}

GLuint gle::Program::getId() const
{
  return (_id);
}

void gle::Program::setUniform(const GLchar* uniform, const Matrix4<GLfloat> & matrix)
{
  glUniformMatrix4fv(_uniformLocations[uniform], 1, GL_FALSE,
		     (const GLfloat*)matrix);
}

void gle::Program::setUniform(const GLchar* uniform, const Matrix3<GLfloat> & matrix)
{
  glUniformMatrix3fv(_uniformLocations[uniform], 1, GL_FALSE,
		     (const GLfloat*)matrix);
}

void gle::Program::setUniform3(const GLchar* uniform, GLfloat* values,
				GLsizeiptr size)
{
  glUniform3fv(_uniformLocations[uniform], size, values);
}

void gle::Program::setUniform1(const GLchar* uniform, GLfloat* values,
				GLsizeiptr size)
{
  glUniform1fv(_uniformLocations[uniform], size, values);
}

void gle::Program::setUniformMatrix4(const GLchar* uniform, GLfloat* values,
				     GLsizeiptr size)
{
  glUniformMatrix4fv(_uniformLocations[uniform],
		     size,
		     false,
		     values);
}

void gle::Program::setUniform1(const GLchar* uniform, GLint* values,
				GLsizeiptr size)
{
  glUniform1iv(_uniformLocations[uniform], size, values);
}

void gle::Program::setUniformMatrix4v(const GLchar* uniform, GLfloat* values,
				      GLsizeiptr count)
{
  glUniformMatrix4fv(_uniformLocations[uniform], count, GL_FALSE, values);
}

void gle::Program::setUniform(const GLchar* uniform,
			      gle::Color<GLfloat> const & color)
{
  glUniform3f(_uniformLocations[uniform], color.r, color.g, color.b);
}

void gle::Program::setUniform(const GLchar* uniform, GLfloat value)
{
  glUniform1f(_uniformLocations[uniform], value);
}

void gle::Program::setUniform(const GLchar* uniform, TextureUnit texture)
{
  glUniform1i(_uniformLocations[uniform], texture);
}

void gle::Program::setUniform(const GLchar* uniform, bool value)
{
  glUniform1i(_uniformLocations[uniform], (value) ? 1 : 0);
}

void gle::Program::setUniform(const GLchar* uniform, const gle::Vector3f& value)
{
  glUniform3f(_uniformLocations[uniform], value.x, value.y, value.z);
}

void gle::Program::retreiveUniformBlockIndex(const std::string &name)
{
  _uniformBlockIndexes[name] = glGetUniformBlockIndex(_id, name.c_str());
  _uniformBlockBindings[name] = _currentUniformBlockBinding;
  glUniformBlockBinding(_id,
                        _uniformBlockIndexes[name],
                        _uniformBlockBindings[name]);
  _currentUniformBlockBinding += 1;
}

GLuint gle::Program::getUniformBlockIndex(const std::string &name)
{
  return (_uniformBlockIndexes[name]);
}

GLuint gle::Program::getUniformBlockBinding(const std::string &name)
{
  return (_uniformBlockBindings[name]);
}
