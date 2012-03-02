//
// ObjLoader.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 22 23:03:27 2012 gael jochaud-du-plessix
// Last update Fri Mar  2 15:43:26 2012 gael jochaud-du-plessix
//

#ifndef _GLE_OBJ_LOADER_HPP_
# define _GLE_OBJ_LOADER_HPP_

# include <string>
# include <vector>
# include <map>
# include <FileLoader.hpp>
# include <Array.hpp>
# include <Material.hpp>
# include <Texture.hpp>

namespace gle {

  //! Class for loading Wavefront .obj files
  /*
    This class permits to load a mesh from a .obj file, through
    the interface provided by FileLoader.
   */

  class ObjLoader : public FileLoader {
  public:

    //! Create an ObjLoader and initialize it

    ObjLoader();

    //! Destruct an ObjLoader
    virtual ~ObjLoader();

    //! Load a mesh from an .obj file
    /*!
      If the file cannot be opened, return NULL.
      If an error occur during the file parsing,
      a ParsingError exception can be thrown.
     */
    gle::Mesh* load(std::string const & file,
		    gle::Material* defaultMaterial);
    
  private:
    void _parseLine(Mesh *parent, std::string const & line);
    void _parseGroup(Mesh* parent, std::vector<std::string> const & lineParts);
    void _parseVertex(std::vector<std::string> const & lineParts);
    void _parseNormal(std::vector<std::string> const & lineParts);
    void _parseTextureCoords(std::vector<std::string> const & lineParts);
    GLfloat _parseFloat(std::string const & value);
    void _parseFace(std::vector<std::string> const & lineParts);
    void _addFaceIndexes(gle::Vector3<GLint> const & index1,
			 gle::Vector3<GLint> const & index2,
			 gle::Vector3<GLint> const & index3);
    std::vector< gle::Vector3<GLint> >
    _parseIndexes(std::vector<std::string> const & lineParts);
    void _addCurrentMesh(Mesh* parent);

    void _parseUseMaterial(std::vector<std::string> const & lineParts);
    void _parseMaterial(std::vector<std::string> const & lineParts);
    std::string _parseFilename(std::string const & filename);
    void _parseMaterialLine(std::string const & line);
    void _parseNewMaterial(std::vector<std::string> const & lineParts);
    void _parseMaterialProperty(std::vector<std::string> const & lineParts);
    void _setMaterialIllumination(int illum);
    void _parseMap(std::vector<std::string> const & lineParts);
    gle::Texture* _getTexture(std::string const & path);

    std::vector<std::string> _explode(std::string const & line,
				      char delimiter, bool skipEmpty=true);
    std::string _epurStr(std::string const str);

    gle::Mesh* _currentMesh;
    std::vector< gle::Vector3<GLfloat> > _currentVertexes;
    std::vector< gle::Vector3<GLfloat> > _currentNormals;
    std::vector< GLfloat > _currentTextures;
    gle::Array<GLuint> _currentVertexesIndexes;
    gle::Array<GLuint> _currentTexturesIndexes;
    gle::Array<GLuint> _currentNormalsIndexes;
    GLuint _currentLine;
    std::string _currentFilename;
    gle::Material* _currentDefaultMaterial;

    gle::Material* _currentMaterial;
    std::string _currentMaterialFilename;
    GLuint _currentMaterialLine;

    gle::Material* _currentUsedMaterial;
    std::map<std::string, gle::Texture*> _loadedTextures;
    std::map<std::string, gle::Material*> _registeredMaterials;
  };
}

#endif
