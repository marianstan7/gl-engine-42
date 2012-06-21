//
// UniversalLoader.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Tue Jun 12 16:02:46 2012 loick michard
// Last update Tue Jun 12 20:20:45 2012 loick michard
//

#ifndef _UNIVERSAL_LOADER_HPP_
# define _UNIVERSAL_LOADER_HPP_

# include <FileLoader.hpp>
# include <Material.hpp>
# include <assimp/assimp.hpp>
# include <assimp/aiPostProcess.h>
# include <assimp/aiScene.h>

namespace gle {

  class UniversalLoader : public FileLoader {
  public:
    UniversalLoader();
    virtual ~UniversalLoader();

    virtual gle::Scene::Node* load(std::string const & file,
				   gle::Material* defaultMaterial);
    void setTexturesPath(const std::string& texturesPath);
  private:
    gle::Material* _loadAssimpMaterial(const std::string& file, const aiScene* scene, aiMaterial* material);
    void _loadAssimpNode(const aiScene* scene, aiNode* node);

    Assimp::Importer		_importer;
    std::vector<gle::Material*> _materials;
    gle::Scene::Node* _rootNode;
    std::string _texturesPath;
  };
};

#endif
