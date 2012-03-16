//
// ObjLoader.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 22 23:17:47 2012 gael jochaud-du-plessix
// Last update Fri Mar 16 17:18:44 2012 gael jochaud-du-plessix
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
  _currentLine(0), _currentFilename(), _currentDefaultMaterial(0),  
  _currentMaterial(0), _currentMaterialFilename(), _currentMaterialLine(0),
  _currentUsedMaterial(NULL),
  _loadedTextures(), _registeredMaterials()
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
    throw new gle::Exception::InvalidValue("Model '"
					   + file + "' not found");
  _currentFilename = file;
  _currentDefaultMaterial = defaultMaterial;
  _currentMesh = NULL;
  _currentVertexes.resize(0);
  _currentTextures.resize(0);
  _currentNormals.resize(0);
  _currentVertexesIndexes.resize(0);
  _currentTexturesIndexes.resize(0);
  _currentNormalsIndexes.resize(0);
  _currentMaterial = NULL;
  _currentUsedMaterial = NULL;
  _registeredMaterials.clear();
  gle::Mesh* parentMesh = new gle::Mesh();
  std::string line;
  _currentLine = 0;
  while (fileStream.good())
    {
      _currentLine++;
      std::getline(fileStream, line);
      size_t dashPos = line.find('#');
      if (dashPos != std::string::npos)
	line = line.substr(0, dashPos);
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
  std::string epured = _epurStr(line);
  if (epured.length() < 1)
    return ;
  if (epured[0] == '#')
    return ;
  std::vector<std::string> parts = _explode(epured, ' ');
  if (parts.size() < 1)
    return ;
  if (parts[0] == "g")
    _parseGroup(parent, parts);
  else if (parts[0] == "v")
    _parseVertex(parts);
  else if (parts[0] == "vn")
    _parseNormal(parts);
  else if (parts[0] == "vt")
    _parseTextureCoords(parts);
  else if (parts[0] == "f")
    _parseFace(parts);
  else if (parts[0] == "mtllib")
    _parseMaterial(parts);
  else if (parts[0] == "usemtl")
    _parseUseMaterial(parts);
}

void gle::ObjLoader::_parseGroup(Mesh* parent,
				 std::vector<std::string> const & lineParts)
{
  if (lineParts.size() < 2)
    return ;
  std::string meshName = lineParts[1];
  gle::Mesh* mesh = new gle::Mesh();
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

void gle::ObjLoader::_parseTextureCoords(std::vector<std::string> const &
					 lineParts)
{
  if (lineParts.size() < 3)
    return ;
  _currentTextures.push_back(_parseFloat(lineParts[1]));
  _currentTextures.push_back(_parseFloat(lineParts[2]));
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
      gle::Array<GLfloat> textureCoords(_currentVertexesIndexes.size());
      gle::Array<GLuint> indexes(_currentVertexesIndexes.size());
      for (size_t i = 0; i + 2 < _currentVertexesIndexes.size(); i += 3)
	{
	  // Compute the normals if we don't have it
	  gle::Vector3<GLfloat> calculatedNormal;
	  if (i + 2 >= _currentNormalsIndexes.size())
	    {
	      gle::Vector3<GLfloat> p1 =
		_currentVertexes[_currentVertexesIndexes[i]];
	      gle::Vector3<GLfloat> p2 =
		_currentVertexes[_currentVertexesIndexes[i + 1]];
	      gle::Vector3<GLfloat> p3 =
		_currentVertexes[_currentVertexesIndexes[i + 2]];
	      gle::Vector3<GLfloat> v1 = p2 - p1;
	      gle::Vector3<GLfloat> v2 = p3 - p1;
	      calculatedNormal = v1 ^ v2;
	      calculatedNormal.normalize();
	    }
	  
	  for (size_t j = 0; j < 3; ++j)
	    {
	      if (_currentVertexesIndexes[i + j] >= _currentVertexes.size())
	      	continue ;
	      vertexes
		.push(_currentVertexes[_currentVertexesIndexes[i + j]]);
	      if (i + j < _currentNormalsIndexes.size()
		  && _currentNormalsIndexes[i + j] < _currentNormals.size())
		normals
		  .push(_currentNormals[_currentNormalsIndexes[i + j]]);
	      else
		normals.push(calculatedNormal);
	      if (i + j < _currentTexturesIndexes.size()
		  && ((_currentTexturesIndexes[i + j] * 2)
		      < _currentTextures.size())
		  && ((_currentTexturesIndexes[i + j] * 2 + 1)
		      < _currentTextures.size()))
		textureCoords
		  .push(_currentTextures[_currentTexturesIndexes[i + j]
					 * 2])
		  .push(_currentTextures[_currentTexturesIndexes[i + j]
					 * 2 + 1]);
	      indexes.push(i + j);
	    }
	}      
      _currentMesh->setVertexes(vertexes);
      _currentMesh->setNormals(normals);
      _currentMesh->setIndexes(indexes);
      if (textureCoords.size() > 0)
	_currentMesh->setTextureCoords(textureCoords);
      if (_currentUsedMaterial)
	_currentMesh->setMaterial(_currentUsedMaterial);
      else if (_registeredMaterials["default"])
	_currentMesh->setMaterial(_registeredMaterials["default"]);
      else
	_currentMesh->setMaterial(_currentDefaultMaterial);
      parent->addChild(_currentMesh);
    }

  _currentVertexesIndexes.resize(0);
  _currentTexturesIndexes.resize(0);
  _currentNormalsIndexes.resize(0);
  _currentMesh = NULL;
}

void gle::ObjLoader::_parseUseMaterial(std::vector<std::string> const &
				       lineParts)
{
  if (lineParts.size() < 2)
    return;
    /*    throw new gle::Exception::ParsingError("Invalid use material",
					   _currentLine,
					   _currentFilename);*/
  gle::Material* material = _registeredMaterials[lineParts[1]];
  if (material)
    _currentUsedMaterial = material;
  else
    throw new gle::Exception::ParsingError("Unkwnown material " + lineParts[1],
					   _currentLine,
					   _currentFilename);
}

void gle::ObjLoader::_parseMaterial(std::vector<std::string> const & lineParts)
{
  if (lineParts.size() < 2)
    return ;
  std::string materialPath = _parseFilename(lineParts[1]);

  std::fstream fileStream(materialPath.c_str());

  if (!fileStream.is_open())
    throw new gle::Exception::ParsingError("Cannot open " + materialPath,
					   _currentLine,
					   _currentFilename);
  _currentMaterialFilename = materialPath;
  std::string line;
  _currentMaterialLine = 0;
  while (fileStream.good())
    {
      _currentMaterialLine++;
      std::getline(fileStream, line);
      size_t dashPos = line.find('#');
      if (dashPos != std::string::npos)
	line = line.substr(0, dashPos);
      _parseMaterialLine(line);
    }
}

std::string gle::ObjLoader::_parseFilename(std::string const & filename)
{
  if (filename[0] != '/')
    {
      size_t slashPos = _currentFilename.find_last_of('/');
      if (slashPos != std::string::npos)
	return(_currentFilename.substr(0, slashPos + 1) + filename);
    }
  return (filename);
}

void gle::ObjLoader::_parseMaterialLine(std::string const & line)
{
  std::string epured = _epurStr(line);
  if (epured.length() < 1)
    return ;
  if (epured[0] == '#')
    return ;
  std::vector<std::string> parts = _explode(epured, ' ');
  if (parts.size() < 1)
    return ;
  if (parts[0] == "newmtl")
    _parseNewMaterial(parts);
  else if (parts[0].compare(0, 4, "map_") == 0 || parts[0] == "bump")
    _parseMap(parts);
  else
    _parseMaterialProperty(parts);
}

void gle::ObjLoader::_parseNewMaterial(std::vector<std::string> const &
				       lineParts)
{
  if (lineParts.size() < 2)
    throw new gle::Exception::ParsingError("Invalid material declaration",
    					   _currentMaterialLine,
    					   _currentMaterialFilename);
  _currentMaterial = new gle::Material();
  _currentMaterial->setName(lineParts[1]);
  _registeredMaterials[lineParts[1]] = _currentMaterial;
}

void gle::ObjLoader::_parseMaterialProperty(std::vector<std::string> const &
					    lineParts)
{
  if (lineParts[0] == "Ka" || lineParts[0] == "Kd" || lineParts[0] == "Ks")
    {
      if (lineParts.size() < 4)
	throw new gle::Exception::
	  ParsingError("Invalid material color declaration",
		       _currentMaterialLine,
		       _currentMaterialFilename);
      gle::Color<GLfloat> color(_parseFloat(lineParts[1]),
				_parseFloat(lineParts[2]),
				_parseFloat(lineParts[3]));
      if (lineParts[0] == "Ka")
	_currentMaterial->setAmbientColor(color);
      else if (lineParts[0] == "Kd")
	_currentMaterial->setDiffuseColor(color);
      else if (lineParts[0] == "Ks")
	_currentMaterial->setSpecularColor(color);      
    }
  if ((lineParts[0] == "Ns" || lineParts[0] == "d" || lineParts[0] == "Tr"
       || lineParts[0] == "illum")
      && lineParts.size() < 2)
    throw new gle::Exception::
      ParsingError("Invalid material property declaration",
		   _currentMaterialLine,
		   _currentMaterialFilename);
  if (lineParts[0] == "Ns")
    {
      _currentMaterial->setShininess(_parseFloat(lineParts[1]));
      return ;
    }
  else if (lineParts[0] == "d" || lineParts[0] == "Tr")
    return ;
  else if (lineParts[0] == "illum")
    _setMaterialIllumination(atoi(lineParts[1].c_str()));
}

void gle::ObjLoader::_setMaterialIllumination(int illum)
{
  if (illum == 0)
    {      
      _currentMaterial->setDiffuseLightEnabled(true);
      _currentMaterial->setSpecularLightEnabled(true);
    }
  else if (illum == 1)
    {
      _currentMaterial->setDiffuseLightEnabled(true);
      _currentMaterial->setSpecularLightEnabled(false);
    }
  else if (illum >= 2)
    {
      _currentMaterial->setDiffuseLightEnabled(true);
      _currentMaterial->setSpecularLightEnabled(true);
    }
}

void gle::ObjLoader::_parseMap(std::vector<std::string> const & lineParts)
{
  if (lineParts.size() < 2)
    return ;
    // throw new gle::Exception::ParsingError("Invalid material map declaration",
    // 					   _currentMaterialLine,
    // 					   _currentMaterialFilename);
  std::string path = _parseFilename(lineParts[1]);
  if (lineParts[0] == "map_Ka" || lineParts[0] == "map_Kd")
    _currentMaterial->setColorMap(_getTexture(path));
}

gle::Texture* gle::ObjLoader::_getTexture(std::string const & path)
{
  gle::Texture* texture = _loadedTextures[path];
  if (texture)
    return (texture);
  texture = new gle::Texture(path);
  if (texture)
    _loadedTextures[path] = texture;
  else
    std::cerr << "Texture '" << path << "' not found" << std::endl;
  return (texture);
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

static inline bool isWhitespace(char c)
{
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

std::string gle::ObjLoader::_epurStr(std::string const str)
{
  std::string result;
  result.reserve(str.size());
  std::string::const_iterator end = str.end();
  std::string::const_iterator it = str.begin();

  for (; it != end && isWhitespace(*it); ++it);
  for (; it != end; ++it)
    {
      if (isWhitespace(*it))
	{
	  for (;isWhitespace(*it) && it != end; ++it);
	  if (it == end)
	    return (result);
	  result += ' ';
	  result += *it;    
	}
      else
	result += *it;
    }
  return (result);
}
