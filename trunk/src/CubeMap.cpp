//
// CubeMap.cpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Fri Jun  8 01:53:33 2012 loick michard
// Last update Thu Jun 21 17:06:57 2012 loick michard
//

#include <CubeMap.hpp>
#include <Exception.hpp>

gle::CubeMap::CubeMap(const Image& posx, const Image& negx, const Image& posy,
		      const Image& negy, const Image& posz, const Image& negz) :
  EnvironmentMap(EnvironmentMap::CubeMap)
{
  if (posx.getWidth() != posx.getHeight() ||
      posy.getWidth() != posy.getHeight() ||
      posz.getWidth() != posz.getHeight() ||
      negx.getWidth() != negx.getHeight() ||
      negy.getWidth() != negy.getHeight() ||
      negz.getWidth() != negz.getHeight() ||
      posx.getWidth() != posy.getWidth() ||
      posy.getWidth() != posz.getWidth() ||
      posz.getWidth() != negx.getWidth() ||
      negx.getWidth() != negy.getWidth() ||
      negy.getWidth() != negz.getWidth())
    throw new gle::Exception::InvalidValue("Invalid cube map texture size");
  _texture = new gle::Texture(0, 0, Texture::CubeMap);
  bind();
  _texture->setData(posx, Texture::PosX, false);
  _texture->setData(negx, Texture::NegX, false);
  _texture->setData(posy, Texture::PosY, false);
  _texture->setData(negy, Texture::NegY, false);
  _texture->setData(posz, Texture::PosZ, false);
  _texture->setData(negz, Texture::NegZ, false);
  _texture->unbind();
}

gle::CubeMap::CubeMap(const std::string& folder, const std::string& extension) :
  CubeMap(folder + "/posx" + extension, folder + "/negx" + extension,
	  folder + "/posy" + extension, folder + "/negy" + extension,
	  folder + "/posz" + extension, folder + "/negz" + extension)
{

}

gle::CubeMap::~CubeMap()
{

}

GLuint gle::CubeMap::getId() const
{
  return (_texture->getId());
}

void gle::CubeMap::bind()
{
  _texture->bind();
}
