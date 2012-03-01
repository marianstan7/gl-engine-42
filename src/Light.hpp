//
// Light.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 16:56:43 2012 loick michard
// Last update Wed Feb 29 17:11:20 2012 loick michard
//

#ifndef _LIGHT_HPP_
# define _LIGHT_HPP_

namespace gle {

  //! Abstract light class
  
  class Light {
  public:

    //! All types of lights

    enum Type {
      DIRECTIONAL = 0,
      POINT = 1
    };

    //! Constructor with type
    /*
      \param type Type of light
    */

    Light(Type type) : _type(type){}

    //! Default constructor

    virtual ~Light()=0;
    
    //! Return light type
    
    Type getType() const{ return (_type); }

  private:
    Type _type;
  };
}

#endif // _LIGHT_HPP_

