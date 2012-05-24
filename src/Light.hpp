//
// Light.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 16:56:43 2012 loick michard
// Last update Thu May 24 10:42:47 2012 loick michard
//

#ifndef _LIGHT_HPP_
# define _LIGHT_HPP_

# include <Scene.hpp>

namespace gle {

  //! Abstract light class
  
  class Light : public Scene::Node {
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

    Light(Type type) : Scene::Node(Scene::Node::Light), _lightType(type){}

    //! Default constructor

    virtual ~Light()=0;
    
    //! Return light type
    
    Type getLightType() const{ return (_lightType); }

    virtual void update() {};

  private:
    Type _lightType;
  };
}

#endif // _LIGHT_HPP_

