//
// CubeMap.hpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Jun  7 23:10:14 2012 loick michard
// Last update Fri Jul  6 02:39:48 2012 loick michard
//

#ifndef _CUBE_MAP_HPP
# define _CUBE_MAP_HPP

#include <EnvironmentMap.hpp>
#include <Image.hpp>
#include <vector>
#include <Texture.hpp>
#include <string>

namespace gle
{
  //! Cube map environment
  /*!
    Example:
    \code
    // Create a cube from a directory
    // Load 6 different images:
    // 	- ./cubemap/posx.png
    // 	- ./cubemap/negx.png
    // 	- ./cubemap/posy.png
    // 	- ./cubemap/negy.png
    // 	- ./cubemap/posz.png
    // 	- ./cubemap/negz.png
    gle::CubeMap cubeMap("./cubemap/", ".png"); 

    scene->setEnvMap(&cubeMap); // Set Cube map to an existing gle::Scene object

    // Create a new gle::Material that will reflect into the cube map
    gle::Material material("reflectMaterial");
    
    // Configure the material to reflect into cubeMap
    material.setEnvMap(&cubeMap);

    // Set up reflection intensity of material into the cube map from 0 (no reflection) to 1 (total reflection)
    material.setReflectionIntensity(0.8);

    mesh.setMaterial(&material); // Set the material to an existing gle::Mesh
    
    \endcode
   */
  class CubeMap : public EnvironmentMap
  {
  public:

    //! Create a cube map
    /*!
      \param posx Positive x image of cube map
      \param negx Negative x image of cube map
      \param posy Positive y image of cube map
      \param negy Negative y image of cube map
      \param posz Positive z image of cube map
      \param negz Negative z image of cube map
    */
    CubeMap(const Image& posx, const Image& negx, const Image& posy,
	    const Image& negy, const Image& posz, const Image& negz);

    //! Create a cube map
    /*!
      \param folder Path of the cube map folder
      \param extension Extension of cube map images
      Load all the cube map images from folder:
	- folder/posx.extension
	- folder/negx.extension
	- folder/posy.extension
	- folder/negy.extension
	- folder/posz.extension
	- folder/negz.extension
     */
    CubeMap(const std::string& folder, const std::string& extension=".jpg");

    //! Destroy cube map
    ~CubeMap();

    //! Return the cubemap map openGL id
    virtual GLuint getId() const;

    //! Bind the environment map
    virtual void bind();

  private:
    gle::Texture* _texture;
  };
};

#endif
