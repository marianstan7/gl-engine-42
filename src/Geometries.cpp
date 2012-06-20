//
// Geometries.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Mon Feb 20 22:34:18 2012 loick michard
// Last update Fri Jun  8 17:16:10 2012 gael jochaud-du-plessix
//

#include <cmath>
#include <Geometries.hpp>
#include <Array.hpp>
#include <vector>

gle::Mesh* gle::Geometries::Cube(gle::Material* material, GLfloat size, bool boundingVolume)
{
  return (Cuboid(material, size, size, size, boundingVolume));
}

gle::Mesh* gle::Geometries::Cuboid(gle::Material* material, 
				   GLfloat width, GLfloat height,
				   GLfloat depth, bool boundingVolume)
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
    0.0,  0.0,  -1.0,
    0.0,  0.0,  -1.0,
    0.0,  0.0,  -1.0,
    0.0,  0.0,  -1.0,

    //! Back face
    0.0,  0.0, 1.0,
    0.0,  0.0, 1.0,
    0.0,  0.0, 1.0,
    0.0,  0.0, 1.0,

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

  GLfloat textureCoords[] = {
    // Front face
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,

    // Back face
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
    0.0, 0.0,

    // Top face
    0.0, 1.0,
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,

    // Bottom face
    1.0, 1.0,
    0.0, 1.0,
    0.0, 0.0,
    1.0, 0.0,

    // Right face
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
    0.0, 0.0,

    // Left face
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0
  };

  gle::Mesh *cuboid = new gle::Mesh(material);
  cuboid->setVertexes(vertexes, sizeof(vertexes) / sizeof(GLfloat), boundingVolume);
  cuboid->setNormals(normals, sizeof(normals) / sizeof(GLfloat));
  cuboid->setTextureCoords(textureCoords,
			   sizeof(textureCoords) / sizeof(GLfloat));
  cuboid->setIndexes(indexes, sizeof(indexes) / sizeof(GLuint));
  return (cuboid);
}

gle::Mesh* gle::Geometries::Sphere(gle::Material* material,
				   GLfloat radius,
				   GLint slices, GLint stacks, bool boundingVolume)
{
  gle::Array<GLfloat> vertexes;
  gle::Array<GLfloat> normals;
  gle::Array<GLuint> indexes;
  gle::Array<GLfloat> uv;

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
	  uv.push_back(1.0 - (st / stacks));
	  uv.push_back(1.0 - sl / slices);
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
  gle::Mesh * mesh = new gle::Mesh(material);
  mesh->setVertexes(vertexes, boundingVolume);
  mesh->setNormals(normals);
  mesh->setTextureCoords(uv);
  mesh->setIndexes(indexes);
  return (mesh);
}


gle::Mesh* gle::Geometries::Plane(gle::Material* material,
				  GLfloat width, GLfloat height,
				  GLint divisionsX, GLint divisionsY, bool boundingVolume)
{
  gle::Array<GLfloat> vertexes;
  gle::Array<GLfloat> normals;
  gle::Array<GLuint> indexes;
  gle::Array<GLfloat> uv;
  
  for (GLfloat y = 0; y <= divisionsY; ++y)
    {
      for (GLfloat x = 0; x <= divisionsX; ++x)
	{	  
	  vertexes.push((-width / 2) + width * (x / divisionsX), 0,
			(-height / 2) + height * (y / divisionsY));
	  normals.push(0, 1, 0);
	  uv.push(x / divisionsX);
	  uv.push(y / divisionsY);
	  if (x != divisionsX && y != divisionsY)
	    {

	      indexes.push(y * (divisionsX + 1) + x,
			   y * (divisionsX + 1) + x + 1,
			   (y + 1) * (divisionsX + 1) + x);
	      indexes.push(y * (divisionsX + 1) + x + 1,
			   (y + 1) * (divisionsX + 1) + x + 1,
			   (y + 1) * (divisionsX + 1) + x);
	    }
	}
    }

  gle::Mesh * mesh = new gle::Mesh(material);
  mesh->setVertexes(vertexes, boundingVolume);
  mesh->setNormals(normals);
  mesh->setTextureCoords(uv);
  mesh->setIndexes(indexes);
  return (mesh);
}
