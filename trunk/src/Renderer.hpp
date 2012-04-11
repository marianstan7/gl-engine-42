//
// Renderer.hpp for  in /home/jochau_g//dev/opengl/glEngine
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 14 17:12:21 2012 gael jochaud-du-plessix
// Last update Wed Apr 11 22:38:57 2012 loick michard
//

#ifndef _GLE_RENDERER_HPP_
# define _GLE_RENDERER_HPP_

# include <string>
# include <Scene.hpp>

namespace gle {

  //! Rendering class
  /*!
    Controls all rendering operations.
    To use it, first set up an OpenGL context and set is as active.
    Then, just create a new Renderer object, set it up and call
    the function "render" to get started.
  */

  class Renderer {
  public:
    
    //! Construct a new renderer
    Renderer();

    //! Destruct a renderer
    ~Renderer();

    //! Clear the context
    void clear();

    //! Render a scene
    void render(Scene* scene);

  private:
    void _renderMesh(gle::Scene* scene, gle::Mesh* mesh);
    void _setCurrentProgram(gle::Scene* scene,
			    gle::Camera* camera);
    void _setMaterialUniforms(gle::Material* material);
    void _setSceneUniforms(gle::Scene* scene, gle::Camera* camera);
    void _setMeshUniforms(gle::Scene* scene, gle::Mesh* mesh);
    
    
    gle::Program* _currentProgram;
    gle::Material* _currentMaterial;
  };

};

#endif /* _GLE_RENDERER_HPP_ */
