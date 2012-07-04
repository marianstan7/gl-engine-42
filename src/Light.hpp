//
// Light.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 16:56:43 2012 loick michard
// Last update Wed Jul  4 19:15:21 2012 loick michard
//

#ifndef _LIGHT_HPP_
# define _LIGHT_HPP_

# include <Scene.hpp>
# include <Texture.hpp>
# include <FrameBuffer.hpp>
# include <Camera.hpp>

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
      //! Light with a position and a direction
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

    //! Return the shadow map texture if shadows are enabled

    virtual gle::Texture*	getShadowMap();

    //! Return the framebuffer used to render the shadow map if shadows are enabled

    virtual gle::FrameBuffer*	getShadowMapFrameBuffer();

    //! Return the camera used to generate the shadow map

    virtual gle::Camera*	getShadowMapCamera();    

  protected:
    //! Type of light
    Type		_lightType;

    //! Shadow map size
    gle::Rectui		_shadowMapSize;

    //! Shadow map
    gle::Texture*	_shadowMap;

    //! Shadow map frame buffer
    gle::FrameBuffer*	_shadowMapFrameBuffer;

    //! Shadow map camera
    gle::Camera*	_shadowMapCamera;
  };
}

#endif // _LIGHT_HPP_
