//
// FileLoader.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 22 22:56:26 2012 gael jochaud-du-plessix
// Last update Thu Mar  1 18:32:35 2012 gael jochaud-du-plessix
//

#ifndef _GLE_FILE_LOADER_HPP_
# define _GLE_FILE_LOADER_HPP_

# include <string>
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
    virtual gle::Mesh* load(std::string const & file,
			    gle::Material* defaultMaterial);
  };

}

#endif /* _GLE_FILE_LOADER_HPP_ */
