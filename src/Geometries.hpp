//
// Geometries.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Mon Feb 20 22:26:19 2012 loick michard
// Last update Fri Mar  9 21:48:20 2012 gael jochaud-du-plessix
//

#ifndef _GEOMETRIES_HPP_
# define _GEOMETRIES_HPP_

# include <Mesh.hpp>
# include <Material.hpp>
# include <Color.hpp>

namespace gle {

  namespace Geometries {
    gle::Mesh* Cube(gle::Material* material, GLfloat size);
    gle::Mesh* Cuboid(gle::Material* material,
		      GLfloat width, GLfloat height, GLfloat depth);
    gle::Mesh* Sphere(gle::Material* material, GLfloat radius,
		      GLint slices = 15, GLint stacks = 15);
    gle::Mesh* Plane(gle::Material* material, GLfloat width, GLfloat height,
		     GLint divisionsX=1, GLint divisionsY=1);
    gle::Mesh* WiredMesh(gle::Mesh* mesh);
  }

}

#endif
