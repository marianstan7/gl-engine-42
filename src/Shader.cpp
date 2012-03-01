//
// Shader.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Feb 17 13:32:59 2012 gael jochaud-du-plessix
// Last update Mon Feb 20 15:41:50 2012 gael jochaud-du-plessix
//

#include <Shader.hpp>
#include <Exception.hpp>

gle::Shader::Shader(Type type, std::string source) :
  _type(type), _id(0)
{
  _id = glCreateShader(_type);
  if (!source.empty())
    {
      setSource(source);
      compile();
    }
}

gle::Shader::~Shader()
{
  glDeleteShader(_id);
}

GLuint gle::Shader::getId() const
{
  return (_id);
}

gle::Shader::Type gle::Shader::getType() const
{
  return (_type);
}

void gle::Shader::setSource(std::string source)
{
  GLint len = source.length();
  const char *source_str = source.c_str();

  glShaderSource(_id, 1, &source_str, &len);
}

void gle::Shader::compile()
{
  GLint status;

  glCompileShader(_id);
  glGetShaderiv(_id, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE)
    {
      std::string infoLog = getInfoLog();
      throw new gle::Exception::CompilationError(infoLog);
    }
}

std::string gle::Shader::getInfoLog() const
{
  GLint length;

  glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length);
  if (length <= 0)
    return ("");
  char *buffer = new char[length];
  glGetShaderInfoLog(_id, length, NULL, buffer);
  std::string infoLog(buffer);
  delete[] buffer;
  return (infoLog);
}
