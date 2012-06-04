//
// Light.cpp for gle in /home/michar_l//glEngine/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Tue Feb 28 09:59:20 2012 loick michard
// Last update Wed May 30 16:53:14 2012 gael jochaud-du-plessix
//

#include <Light.hpp>

gle::Light::~Light()
{

}

gle::Light::Light(Type type) :
  Scene::Node(Scene::Node::Light), _lightType(type)
{
}

gle::Light::Type gle::Light::getLightType() const
{
  return (_lightType);
}

void gle::Light::update()
{
}
