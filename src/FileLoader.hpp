//
// FileLoader.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 22 22:56:26 2012 gael jochaud-du-plessix
// Last update Tue Jun 12 16:26:30 2012 loick michard
//

#ifndef _GLE_FILE_LOADER_HPP_
# define _GLE_FILE_LOADER_HPP_

# include <string>
# include <Scene.hpp>
# include <Mesh.hpp>

namespace gle {

  //! Class for loading a 3D model from a file
  class FileLoader {
  public:

    //! Construct a FileLoader
    FileLoader();

    //! Destruct a FileLoader
    virtual ~FileLoader();

    //! Load a mesh from a file
    virtual gle::Scene::Node* load(std::string const & file,
				   gle::Material* defaultMaterial);
  };

}

#endif /* _GLE_FILE_LOADER_HPP_ */
