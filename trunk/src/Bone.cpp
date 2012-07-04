//
// Bone.cpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sun Jun 24 12:43:55 2012 loick michard
// Last update Tue Jul  3 16:58:48 2012 gael jochaud-du-plessix
//

#include <Bone.hpp>
#include <Renderer.hpp>
#include <Geometries.hpp>
#include <Mesh.hpp>

gle::Bone::Bone() :
  gle::Scene::Node(gle::Scene::Node::Bone), _size(0)
{

}

gle::Bone::Bone(GLfloat size) :
  gle::Scene::Node(gle::Scene::Node::Bone), _size(size), _debugMaterial(NULL)
{

}

gle::Bone::~Bone()
{

}

void gle::Bone::update()
{
  if (_debugMeshes.size())
    {
      Matrix4<GLfloat> mvMatrix = getTransformationMatrix();
      Matrix3<GLfloat> normalMatrix;
      {
	mvMatrix.translate(Vector3f(0, 0, 0));
	Matrix4<GLfloat> inverse(mvMatrix);
	inverse.inverse();
	normalMatrix = inverse;
	normalMatrix.transpose();
	_debugMeshes[0]->setMatrices(mvMatrix, normalMatrix);
	_debugMeshes[2]->setMatrices(mvMatrix, normalMatrix);
      }
      mvMatrix = getTransformationMatrix();
      {
	mvMatrix.translate(Vector3f(0, _size, 0));
	Matrix4<GLfloat> inverse(mvMatrix);
	inverse.inverse();
	normalMatrix = inverse;
	normalMatrix.transpose();
	_debugMeshes[1]->setMatrices(mvMatrix, normalMatrix);
      }
    }
}

std::vector<gle::Scene::Node*>& gle::Bone::getDebugNodes(int mode)
{
  if (mode & Renderer::Bone)
    {
      if (!_debugMaterial)
	{
	  _debugMaterial = new Material();
	  _debugMaterial->setAmbientColor(Colorf(1.0, 0.0, 1.0));
	}
      if (_debugMeshes.size() == 0)
	{
	  if (_size)
	    {
	      Scene::Node* mesh = Geometries::Sphere(_debugMaterial, _size / 10, 30, 30, true);
	      Scene::Node* mesh2 = Geometries::Sphere(_debugMaterial, _size / 10, 30, 30, true);
	      gle::Mesh* link = new gle::Mesh(_debugMaterial, true);
	      link->setRasterizationMode(Mesh::Line);
	      GLfloat vtx[] = {0, 0, 0, 0, _size, 0, 0, 0, 0};
	      GLuint idx[] = {0, 1, 2};
	      link->setVertexes(vtx, 9);
	      link->setIndexes(idx, 3);
	      
	      Matrix4<GLfloat> mvMatrix = getTransformationMatrix();
	      Matrix3<GLfloat> normalMatrix;
	      {
		mvMatrix.translate(Vector3f(0, 0, 0));
		Matrix4<GLfloat> inverse(mvMatrix);
		inverse.inverse();
		normalMatrix = inverse;
		normalMatrix.transpose();
		mesh->setMatrices(mvMatrix, normalMatrix);
		link->setMatrices(mvMatrix, normalMatrix);
	      }
	      mvMatrix = getTransformationMatrix();
	      {
		mvMatrix.translate(Vector3f(0, _size, 0));
		Matrix4<GLfloat> inverse(mvMatrix);
		inverse.inverse();
		normalMatrix = inverse;
		normalMatrix.transpose();
		mesh2->setMatrices(mvMatrix, normalMatrix);
	      }
	      
	      _debugMeshes.push_back(mesh);
	      _debugMeshes.push_back(mesh2);
	      _debugMeshes.push_back(link);
	    }
	}
    }
  return (_debugMeshes);
}
