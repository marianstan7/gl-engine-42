//
// ObjLoader.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 22 23:17:47 2012 gael jochaud-du-plessix
// Last update Thu Mar  1 18:36:20 2012 gael jochaud-du-plessix
//

#include <fstream>
#include <sstream>
#include <cmath>
#include <ObjLoader.hpp>
#include <cstdlib>

gle::ObjLoader::ObjLoader() :
  _currentMesh(NULL),
  _currentVertexes(0), _currentNormals(0), _currentTextures(0),
  _currentVertexesIndexes(0), _currentTexturesIndexes(0),
  _currentNormalsIndexes(0),
  _currentLine(0), _currentFilename(), _currentDefaultMaterial(0)
{
}

gle::ObjLoader::~ObjLoader()
{
  
}

gle::Mesh* gle::ObjLoader::load(std::string const & file,
				gle::Material* defaultMaterial)
{
  std::fstream fileStream(file.c_str());

  if (!fileStream.is_open())
    return (NULL);
  _currentFilename = file;
  _currentDefaultMaterial = defaultMaterial;
  _currentMesh = NULL;
  _currentVertexes.resize(0);
  _currentTextures.resize(0);
  _currentNormals.resize(0);
  _currentVertexesIndexes.resize(0);
  _currentTexturesIndexes.resize(0);
  _currentNormalsIndexes.resize(0);
  gle::Mesh* parentMesh = new gle::Mesh();
  std::string line;
  _currentLine = 0;
  while (fileStream.good())
    {
      _currentLine++;
      std::getline(fileStream, line);
      _parseLine(parentMesh, line);
    }
  if (_currentMesh != NULL)
    _addCurrentMesh(parentMesh);
  _currentVertexes.clear();
  _currentTextures.clear();
  _currentNormals.clear();
  _currentVertexesIndexes.clear();
  _currentTexturesIndexes.clear();
  _currentNormalsIndexes.clear();
  return (parentMesh);
}

void gle::ObjLoader::_parseLine(Mesh* parent, std::string const & line)
{
  if (line.length() < 1)
    return ;
  if (line[0] == '#')
    return ;
  std::vector<std::string> parts = _explode(line, ' ');
  if (parts.size() < 1)
    return ;
  if (parts[0] == "g")
    _parseGroup(parent, parts);
  else if (parts[0] == "v")
    _parseVertex(parts);
  else if (parts[0] == "vn")
    _parseNormal(parts);
  else if (parts[0] == "f")
    _parseFace(parts);
}

void gle::ObjLoader::_parseGroup(Mesh* parent,
				 std::vector<std::string> const & lineParts)
{
  if (lineParts.size() < 2)
    return ;
  std::string meshName = lineParts[1];
  gle::Mesh* mesh = new gle::Mesh();
  mesh->setMaterial(_currentDefaultMaterial);
  mesh->setName(meshName);
  if (_currentMesh != NULL)
    _addCurrentMesh(parent);
  _currentMesh = mesh;
}

void gle::ObjLoader::_parseVertex(std::vector<std::string> const & lineParts)
{
  // We increment the index counter, in order to calculate relative indexes
  if (lineParts.size() < 4)
    return ;
  gle::Vector3<GLfloat> vertex(_parseFloat(lineParts[1]),
			       _parseFloat(lineParts[2]),
			       _parseFloat(lineParts[3]));
  _currentVertexes.push_back(vertex);
}

void gle::ObjLoader::_parseNormal(std::vector<std::string> const & lineParts)
{
  if (lineParts.size() < 4)
    return ;
  gle::Vector3<GLfloat> normal(_parseFloat(lineParts[1]),
			       _parseFloat(lineParts[2]),
			       _parseFloat(lineParts[3]));
  _currentNormals.push_back(normal);
}

GLfloat gle::ObjLoader::_parseFloat(std::string const & value)
{
  size_t ePos = value.find('e');
  if (ePos != std::string::npos)
    {
      double number = atof(value.substr(0, ePos).c_str());
      double power = atof(value.substr(ePos + 1).c_str());
      return (number * pow(10, power));
    }
  else
    return (atof(value.c_str()));
  return (0);
}

void gle::ObjLoader::_parseFace(std::vector<std::string> const & lineParts)
{
  if (lineParts.size() < 4 || lineParts.size() > 5)
    return ;
  GLuint nbIndexes = lineParts.size() - 1;
  if (!(nbIndexes == 3 || nbIndexes == 4))
    throw new gle::Exception::ParsingError("Invalid face declaration",
					   _currentLine,
					   _currentFilename);
  std::vector< gle::Vector3<GLint> > indexes = _parseIndexes(lineParts);

  // We must have at least a vertex index
  if (indexes[0].x == -1 || indexes[1].x == -1 || indexes[2].x == -1)
    throw new gle::Exception::ParsingError("Vertex index unspecified",
					   _currentLine,
					   _currentFilename);

  if (nbIndexes == 4)
    {
      _addFaceIndexes(indexes[0], indexes[1], indexes[2]);
      _addFaceIndexes(indexes[0], indexes[2], indexes[3]);
    }
  else
    _addFaceIndexes(indexes[0], indexes[1], indexes[2]);
}

void gle::ObjLoader::_addFaceIndexes(gle::Vector3<GLint> const & index1,
				     gle::Vector3<GLint> const & index2,
				     gle::Vector3<GLint> const & index3)
{
  _currentVertexesIndexes.push(index1.x, index2.x, index3.x);
  if (index1.y != -1 && index2.y != -1 && index3.y != -1)
  _currentTexturesIndexes.push(index1.y, index2.y, index3.y);
  if (index1.z != -1 && index2.z != -1 && index3.z != -1)
    _currentNormalsIndexes.push(index1.z, index2.z, index3.z);
}

std::vector< gle::Vector3<GLint> >
gle::ObjLoader::_parseIndexes(std::vector<std::string> const & lineParts)
{
  std::vector< gle::Vector3<GLint> > result(0);
  for (std::vector<std::string>::const_iterator
	 it = ++lineParts.begin(),
	 end = lineParts.end(); it != end; ++it)
    {
      std::vector<std::string> parts = _explode(*it, '/', false);
      int nbParts = parts.size();
      gle::Vector3<GLint> values(
				  atoi(parts[0].c_str()) - 1,
				  nbParts > 1 ?
				  atoi(parts[1].c_str()) - 1 : -1,
				  nbParts > 2 ?
				  atoi(parts[2].c_str()) - 1 : -1
				  );
      result.push_back(values);
    }
  return (result);
}

void gle::ObjLoader::_addCurrentMesh(Mesh* parent)
{
  if (!_currentMesh)
    return ;
  if (_currentVertexesIndexes.size() > 0)
    {
      gle::Array<GLfloat> vertexes(_currentVertexesIndexes.size() * 3);
      gle::Array<GLfloat> normals(_currentVertexesIndexes.size() * 3);
      gle::Array<GLuint> indexes(_currentVertexesIndexes.size());
      for (size_t i = 0; i + 2 < _currentVertexesIndexes.size(); i += 3)
	{
	  if (i + 2 < _currentNormalsIndexes.size())
	    {
	      for (size_t j = 0; j < 3; ++j)
		{
		  vertexes
		    .push(_currentVertexes[_currentVertexesIndexes[i + j]]);
		  normals
		    .push(_currentNormals[_currentNormalsIndexes[i + j]]);
		  indexes.push(i + j);
		}
	    }
	  // Compute the normals if we don't have it
	  else
	    {
	      gle::Vector3<GLfloat> p1 =
		_currentVertexes[_currentVertexesIndexes[i]];
	      gle::Vector3<GLfloat> p2 =
		_currentVertexes[_currentVertexesIndexes[i + 1]];
	      gle::Vector3<GLfloat> p3 =
		_currentVertexes[_currentVertexesIndexes[i + 2]];
	      gle::Vector3<GLfloat> v1 = p2 - p1;
	      gle::Vector3<GLfloat> v2 = p3 - p1;
	      gle::Vector3<GLfloat> calculatedNormal = v1 ^ v2;
	      calculatedNormal.normalize();
	      for (size_t j = 0; j < 3; ++j)
		{
		  vertexes
		    .push(_currentVertexes[_currentVertexesIndexes[i + j]]);
		  normals.push(calculatedNormal);
		  indexes.push(i + j);
		}	      
	    }
	}
      _currentMesh->setVertexes(vertexes);
      _currentMesh->setNormals(normals);
      _currentMesh->setIndexes(indexes);
      parent->addChild(_currentMesh);
    }
  _currentVertexesIndexes.resize(0);
  _currentTexturesIndexes.resize(0);
  _currentNormalsIndexes.resize(0);
  _currentMesh = NULL;
}

std::vector<std::string> gle::ObjLoader::_explode(std::string const & line,
						  char delimiter,
						  bool skipEmpty)
{
  std::vector<std::string> parts;
  std::stringstream stringStream(line);
  std::string part;
  while (stringStream.good())
    {
      std::getline(stringStream, part, delimiter);
      if (!skipEmpty || part.length() > 0)
	parts.push_back(part);
    }
  return (parts);
}
