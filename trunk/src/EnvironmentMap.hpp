//
// EnvironmentMap.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Jun  7 23:04:00 2012 loick michard
// Last update Sun Jun 10 14:00:28 2012 loick michard
//

#ifndef _ENVIRONMENT_MAP_HPP_
# define _ENVIRONMENT_MAP_HPP_

# include <gle/opengl.h>

namespace gle {
  class EnvironmentMap {
  public:
    enum Type {
      CubeMap = 1
    };
    EnvironmentMap(Type type) : _type(type) {}
    virtual ~EnvironmentMap(){};
    
    virtual GLuint getId() const = 0;
    virtual void bind() = 0;
    
    Type getType() const {
      return (_type);
    }
  protected:
    Type _type;
  };
};

#endif
