//
// EnvironmentMap.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Jun  7 23:04:00 2012 loick michard
// Last update Wed Jul  4 17:16:58 2012 loick michard
//

#ifndef _ENVIRONMENT_MAP_HPP_
# define _ENVIRONMENT_MAP_HPP_

# include <gle/opengl.h>

namespace gle {
  //! Handler of environment map
  class EnvironmentMap {
  public:
    //! Environment map type
    enum Type {
      CubeMap = 1
    };

    //! Create an environment map
    /*!
      \param type Type
     */
    EnvironmentMap(Type type) : _type(type) {}

    //! Destroy environment map
    virtual ~EnvironmentMap(){};
    
    //! Return the environement map openGL id
    virtual GLuint getId() const = 0;

    //! Bind the environment map
    virtual void bind() = 0;
    
    //! Return environment map type
    Type getType() const {
      return (_type);
    }
  protected:
    //! Environment type
    Type _type;
  };
};

#endif
