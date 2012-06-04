//
// Exception.cpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu May 31 18:02:43 2012 gael jochaud-du-plessix
// Last update Thu May 31 18:05:31 2012 gael jochaud-du-plessix
//

#include "Exception.hpp"
#include <gle/opengl.h>

void gle::Exception::CheckOpenGLError(const std::string& message)
{
  GLuint    error = glGetError();

  if (error == GL_NO_ERROR)
    return ;
  else if (error == GL_INVALID_ENUM)
    {
      if (!message.empty())
	throw new InvalidEnum(message);
      else
	throw new InvalidEnum();
    }
  else if (error == GL_INVALID_VALUE)
    {
      if (!message.empty())
	throw new InvalidValue(message);
      else
	throw new InvalidValue();
    }
  else if (error == GL_INVALID_OPERATION)
    {
      if (!message.empty())
	throw new InvalidOperation(message);
      else
	throw new InvalidOperation();
    }
  else if (error == GL_OUT_OF_MEMORY)
    {
      if (!message.empty())
	throw new OutOfMemory(message);
      else
	throw new OutOfMemory();
    }
  else
    {
      if (!message.empty())
	throw new OpenGLError(message);
      else
	throw new OpenGLError();
    }
}
