//
// Light.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 16:56:43 2012 loick michard
// Last update Wed Jun  6 15:52:15 2012 loick michard
//

#ifndef _LIGHT_HPP_
# define _LIGHT_HPP_

# include <Scene.hpp>

namespace gle {

  //! Abstract light class
  
  class Light : public Scene::Node {
  public:

    //! Different types of light

    enum Type {
      //! Light with the same angle at any point
      DIRECTIONAL = 0,
      //! Light representing a omnidirectional lighting point
      POINT = 1,
      SPOT = 2
    };

    //! Constructor with type
    /*
      \param type Type of light
    */

    Light(Type type);

    //! Destructor to implement

    virtual ~Light() = 0;
    
    //! Return light type
    
    Type getLightType() const;

    //! Update the light

    virtual void update();

  private:
    Type _lightType;
  };
}

#endif // _LIGHT_HPP_
