//
// Light.cpp for gle in /home/michar_l//glEngine/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Tue Feb 28 09:59:20 2012 loick michard
// Last update Tue Jul 10 14:24:06 2012 loick michard
//

#include <Light.hpp>

gle::Light::Light(Type type) :
  Scene::Node(Scene::Node::Light), _lightType(type),
  _shadowMapSize(0, 0, 1024, 1024),
  _shadowMap(), _shadowMapFrameBuffer(), _shadowMapCamera()
{
}

gle::Light::~Light()
{
  if (_shadowMap)
    delete _shadowMap;
  if (_shadowMapFrameBuffer)
    delete _shadowMapFrameBuffer;
  if (_shadowMapCamera)
    delete _shadowMapCamera;
}

gle::Light::Type gle::Light::getLightType() const
{
  return (_lightType);
}

void gle::Light::update()
{
}

gle::Texture*	gle::Light::getShadowMap()
{
  if (!_projectShadow)
    return (NULL);
  if (!_shadowMap)
    {
      _shadowMap = new gle::Texture(_shadowMapSize.width, _shadowMapSize.height,
				    gle::Texture::Texture2D, gle::Texture::Depth);
      _shadowMap->setUseMipmap(false);
      _shadowMap->setFilterType(gle::Texture::Nearest);      
      //_shadowMap->setWrapMode(gle::Texture::Clamp);
    }
  return (_shadowMap);
}

gle::FrameBuffer*	gle::Light::getShadowMapFrameBuffer()
{
  if (!_projectShadow)
    return (NULL);
  if (!_shadowMapFrameBuffer)
    {
      _shadowMapFrameBuffer = new gle::FrameBuffer();
      _shadowMapFrameBuffer->attach(*getShadowMap(), gle::FrameBuffer::AttachmentDepth);
    }
  return (_shadowMapFrameBuffer);
}

gle::Camera* gle::Light::getShadowMapCamera()
{
  return (_shadowMapCamera);
}
