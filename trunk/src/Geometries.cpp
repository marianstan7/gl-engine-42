//
// Geometries.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Mon Feb 20 22:34:18 2012 loick michard
// Last update Tue Feb 28 17:00:55 2012 gael jochaud-du-plessix
//

#include <cmath>
#include <Geometries.hpp>
#include <Array.hpp>
#include <vector>

gle::Mesh* gle::Geometries::Cube(gle::Material* material, GLfloat size)
{
  return (Cuboid(material, size, size, size));
}

gle::Mesh* gle::Geometries::Cuboid(gle::Material* material, 
				   GLfloat width, GLfloat height,
				   GLfloat depth)
{
  GLfloat halfWidth = width / 2;
  GLfloat halfHeight = height / 2;
  GLfloat halfDepth = depth / 2;
  
  GLfloat vertexes[] = {
    //! Top face
   -halfWidth, halfHeight,  -halfDepth,
   halfWidth, halfHeight,  -halfDepth,
   halfWidth, halfHeight,  halfDepth,
   -halfWidth, halfHeight,  halfDepth,
   
    //! bottom face
   -halfWidth, -halfHeight,  -halfDepth,
   halfWidth, -halfHeight,  -halfDepth,
   halfWidth, -halfHeight,  halfDepth,
   -halfWidth, -halfHeight,  halfDepth,

    //! Front face
   -halfWidth, halfHeight,  -halfDepth,
   -halfWidth, -halfHeight,  -halfDepth,
   halfWidth, -halfHeight,  -halfDepth,
   halfWidth, halfHeight,  -halfDepth,

    //! Back face
   -halfWidth, -halfHeight,  halfDepth,
   halfWidth, -halfHeight,  halfDepth,
   halfWidth, halfHeight,  halfDepth,
   -halfWidth, halfHeight,  halfDepth,

    //! Right face
   halfWidth, -halfHeight,  -halfDepth,
   halfWidth, -halfHeight,  halfDepth,
   halfWidth, halfHeight,  halfDepth,
   halfWidth, halfHeight,  -halfDepth,

    //! Left face
   -halfWidth, halfHeight,  -halfDepth,
   -halfWidth, -halfHeight,  -halfDepth,
   -halfWidth, -halfHeight,  halfDepth,
   -halfWidth, halfHeight,  halfDepth,
  };
  
  GLuint indexes[] = {
    0, 1, 2, 0, 2, 3, //! Front face
    4, 5, 6, 4, 6, 7, //! Back face
    8, 9, 10, 8, 10, 11, //! Top face
    12, 13, 14, 12, 14, 15, //! Bottom face
    16, 17, 18, 16, 18, 19, //! Right face
    20, 21, 22, 20, 22, 23 //! Left face
  };

  GLfloat normals[] = {
    //! Top face
    0.0,  1.0,  0.0,
    0.0,  1.0,  0.0,
    0.0,  1.0,  0.0,
    0.0,  1.0,  0.0,

    //! Bottom face
    0.0, -1.0,  0.0,
    0.0, -1.0,  0.0,
    0.0, -1.0,  0.0,
    0.0, -1.0,  0.0,

    //! Front face
    0.0,  0.0,  1.0,
    0.0,  0.0,  1.0,
    0.0,  0.0,  1.0,
    0.0,  0.0,  1.0,

    //! Back face
    0.0,  0.0, -1.0,
    0.0,  0.0, -1.0,
    0.0,  0.0, -1.0,
    0.0,  0.0, -1.0,

    //! Right face
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,

    //! Left face
    -1.0,  0.0,  0.0,
    -1.0,  0.0,  0.0,
    -1.0,  0.0,  0.0,
    -1.0,  0.0,  0.0
  };

  return (new gle::Mesh(material,
			vertexes, sizeof(vertexes) / sizeof(GLfloat),
			normals, sizeof(normals) / sizeof(GLfloat),
			indexes, sizeof(indexes) / sizeof(GLuint)));
}

gle::Mesh* gle::Geometries::Sphere(gle::Material* material,
				   GLfloat radius,
				   gle::Color<GLfloat> const& color,
				   GLint slices, GLint stacks)
{
  std::vector<GLfloat> vertexes;
  std::vector<GLfloat> normals;
  std::vector<GLuint> indexes;
  std::vector<GLfloat> colors;

  for (GLfloat sl = 0; sl <= slices; ++sl)
    {
      GLfloat theta = sl * M_PI / slices;
      for (GLfloat st = 0; st <= stacks; ++st)
	{
	  GLfloat phi = st * 2.0 * M_PI / stacks;
	  GLfloat x = sin(theta) * cos(phi);
	  GLfloat y = cos(theta);
	  GLfloat z = sin(theta) * sin(phi);

	  vertexes.push_back(x * radius);
	  vertexes.push_back(y * radius);
	  vertexes.push_back(z * radius);
	  normals.push_back(x);
	  normals.push_back(y);
	  normals.push_back(z);
	  colors.push_back(color.r);
	  colors.push_back(color.g);
	  colors.push_back(color.b);
	  colors.push_back(color.a);
	  if (sl != slices && st != stacks)
	    {
	      GLfloat indice1 = sl * (stacks + 1) + st;
	      GLfloat indice2 = indice1 + stacks + 1;
	      indexes.push_back(indice1);
	      indexes.push_back(indice2);
	      indexes.push_back(indice1 + 1);
	      indexes.push_back(indice2);
	      indexes.push_back(indice2 + 1);
	      indexes.push_back(indice1 + 1);
	    }
	}
    }
  return (new gle::Mesh(material, &vertexes[0], vertexes.size(),
			&normals[0], normals.size(),
			&indexes[0], indexes.size(),
			&colors[0], colors.size()));
}
