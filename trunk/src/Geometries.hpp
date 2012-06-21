//
// Geometries.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Mon Feb 20 22:26:19 2012 loick michard
// Last update Mon Jun 18 19:20:53 2012 loick michard
//

#ifndef _GEOMETRIES_HPP_
# define _GEOMETRIES_HPP_

# include <Mesh.hpp>
# include <Material.hpp>
# include <Color.hpp>

namespace gle {

  namespace Geometries {
    gle::Mesh* Cube(gle::Material* material, GLfloat size, bool boundingVolume = true);
    gle::Mesh* Cuboid(gle::Material* material,
		      GLfloat width, GLfloat height, GLfloat depth, bool boundingVolume = true);
    gle::Mesh* Sphere(gle::Material* material, GLfloat radius,
		      GLint slices = 15, GLint stacks = 15, bool boundingVolume = true);
    gle::Mesh* Plane(gle::Material* material, GLfloat width, GLfloat height,
		     GLint divisionsX=1, GLint divisionsY=1,
		     GLint textureX=1, GLint textureY=1, bool boundingVolume = true);
  }

}

#endif
