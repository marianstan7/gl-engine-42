//
// CubeMap.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Jun  7 23:10:14 2012 loick michard
// Last update Sun Jun 17 22:11:36 2012 loick michard
//

#ifndef _CUBE_MAP_HPP
# define _CUBE_MAP_HPP

#include <EnvironmentMap.hpp>
#include <Image.hpp>
#include <vector>
#include <Texture.hpp>
#include <string>

namespace gle
{
  class CubeMap : public EnvironmentMap
  {
  public:
    CubeMap(const Image& posx, const Image& negx, const Image& posy,
	    const Image& negy, const Image& posz, const Image& negz);
    CubeMap(const std::string& folder, const std::string& extension=".jpg");
    ~CubeMap();

    virtual GLuint getId() const;
    virtual void bind();

  private:
    gle::Texture* _texture;
  };
};

#endif
