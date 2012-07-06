//
// UniversalLoader.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Tue Jun 12 16:02:46 2012 loick michard
// Last update Fri Jul  6 03:01:30 2012 loick michard
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
  /*!
    Example:
    \code
    // Creating a gle::UniversalLoader instance
    gle::UniversalLoader loader();

    // Load a model from his filename and set a default material for model importation
    gle::Scene:::Node* model = loader.load("models/camaro.obj", new Material()); // Yes, this is an awesome camaro

    // Retreive a special node into the model by its name
    gle::Scene::Node* steeringWheel = model->getChildByName("steeringWheel");

    // Rotate the retreive node of 45 degrees around the (1.0, 1.0, 0.0) axis
    steeringWheel->setRotation(gle::Vector3f(1.0, 1.0, 0.0), 45);

    // Add the model to an existing gle::Scene
    scene << model;
    \endcode
   */
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
