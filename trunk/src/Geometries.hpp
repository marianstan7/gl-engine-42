//
// Geometries.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Mon Feb 20 22:26:19 2012 loick michard
// Last update Thu Jul  5 22:48:07 2012 gael jochaud-du-plessix
//

#ifndef _GEOMETRIES_HPP_
# define _GEOMETRIES_HPP_

# include <Mesh.hpp>
# include <Material.hpp>
# include <Color.hpp>

namespace gle {

  //! Utilities functions for creating meshes
  namespace Geometries {

    //! Creates a cube
    /*!
      \param material The material of the cube
      \param size Size of a side of the cube
      \param isDynamic Indicates whether or not the cube is dynamic
     */
    gle::Mesh* Cube(gle::Material* material, GLfloat size, bool isDynamic = false);

    //! Creates a cuboid
    /*!
      \param material The material of the cuboid
      \param width Size of the cuboid on the \c X axis
      \param height Size of the cuboid on the \c Y axis
      \param depth Size of the cuboid on the \c Z axis
      \param isDynamic Indicates whether or not the cuboid is dynamic
     */
    gle::Mesh* Cuboid(gle::Material* material,
		      GLfloat width, GLfloat height, GLfloat depth, bool isDynamic = false);

    //! Creates a sphere
    /*!
      \param material The material of the sphere
      \param radius Radius of the sphere
      \param slices Number of vertical subdivisions of the sphere
      \param stacks Number of horizontal subdivisions of the sphere
      \param isDynamic Indicates whether or not the sphere is dynamic
    */
    gle::Mesh* Sphere(gle::Material* material, GLfloat radius,
		      GLint slices = 15, GLint stacks = 15, bool isDynamic = false);

    //! Creates a plane
    /*!
      \param material The material of the plane
      \param width Size of the plane on the \c X axis
      \param height Size of the plane on the \c Z axis
      \param divisionsX Number of divisions of the plane on the \c X axis
      \param divisionsZ Number of divisions of the plane on the \c Z axis
      \param textureX Number of repetitions of the texture on the \c X axis
      \param textureZ Number of repetitions of the texture on the \c Z axis
      \param isDynamic Indicates whether or not the plane is dynamic
    */
    gle::Mesh* Plane(gle::Material* material, GLfloat width, GLfloat height,
		     GLint divisionsX=1, GLint divisionsZ=1,
		     GLint textureX=1, GLint textureZ=1, bool isDynamic = false);
  }

}

#endif
