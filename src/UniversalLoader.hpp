//
// UniversalLoader.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Tue Jun 12 16:02:46 2012 loick michard
// Last update Wed Jul  4 17:39:31 2012 gael jochaud-du-plessix
//

#ifndef _UNIVERSAL_LOADER_HPP_
# define _UNIVERSAL_LOADER_HPP_

# include <FileLoader.hpp>
# include <Material.hpp>
# include <Skeleton.hpp>
# include <Bone.hpp>
# include <assimp/assimp.hpp>
# include <assimp/aiPostProcess.h>
# include <assimp/aiScene.h>

namespace gle {

  //! Loader of scene nodes that uses the ASSIMP importer

  class UniversalLoader : public FileLoader {
  public:

    //! Construct a loader

    UniversalLoader();

    //! Destruct a loader

    virtual ~UniversalLoader();

    //! Load a file
    /*!
      Load meshes from a file. Supported format are defined by
      the ASSIMP library capabilities.
      \param file Filename to import
      \param defaultMaterial Default material to use if no specified
      \return The imported scene node with all elements loaded from the file
     */

    virtual gle::Scene::Node* load(std::string const & file,
				   gle::Material* defaultMaterial);

    //! Set the path where the textures must be retrieved

    void setTexturesPath(const std::string& texturesPath);

  private:
    gle::Material*	_loadAssimpMaterial(const std::string& file, const aiScene* scene, aiMaterial* material);
    void		_loadAssimpNode(const aiScene* scene, aiNode* node);
    gle::Skeleton*	_loadAssimpSkeleton(const aiScene* scene, aiMesh* mesh);

    Assimp::Importer		_importer;
    std::vector<gle::Material*> _materials;
    gle::Scene::Node*		_rootNode;
    std::string			_texturesPath;
  };
};

#endif
