//
// Renderer.hpp for  in /home/jochau_g//dev/opengl/glEngine
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 14 17:12:21 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 19:37:35 2012 gael jochaud-du-plessix
//

#ifndef _GLE_RENDERER_HPP_
# define _GLE_RENDERER_HPP_

# include <string>
# include <Scene.hpp>
# include <Mesh.hpp>
# include <Camera.hpp>
# include <FrameBuffer.hpp>
# include <Rect.hpp>
# include <Light.hpp>

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

    //! Elements that can be rendered for debuging

    enum DebugMode {
      BoundingVolume	= 1 << 0,
      Octree		= 1 << 1,
      Bone		= 1 << 2,
      Light		= 1 << 3,
      Camera		= 1 << 4
    };
    
    //! Construct a new renderer

    Renderer();

    //! Destruct a renderer

    ~Renderer();

    //! Clear the frame buffer

    void clear();

    //! Render a scene
	/*!
		Render a scene in a specific Framebuffer, or in the default one by default.
		\param scene Pointer to the scene to render
		\param size Size of the viewport to render
		\param customFramebuffer Pointer to the framebuffer in which to render. NULL to render to the default framebuffer
	*/

    void render(Scene* scene, const Rectf& size, FrameBuffer* customFramebuffer=NULL);

    //! Render a set of static and dynamic meshes to a shadow map
    
    void renderShadowMap(gle::Scene* scene, const std::list<gle::Mesh*> & staticMeshes,
			 const std::list<gle::Mesh*> & dynamicMeshes, gle::Light* light);

    //! Set the debug mode of the renderer
    /*!
      \param mode Bitfield specifying the debug mode, using one or several DebugMode with OR operator
     */

    void setDebugMode(int mode);

  private:
    void _buildIndexesBuffer(const std::list<gle::Mesh*> & meshes);
    void _renderEnvMap(gle::Scene* scene);
    void _renderShadowMapMeshes(gle::Scene::MeshGroup& group);
    void _renderMeshes(gle::Scene* scene, gle::Scene::MeshGroup& group);
    void _renderMesh(gle::Mesh* mesh);
    void _setVertexAttributes(GLuint offset);
    void _setCurrentProgram(gle::Scene* scene);
    void _setMaterialUniforms(gle::Material* material);
    void _setSceneUniforms(gle::Scene* scene, gle::Camera* camera);
    void _setMeshUniforms(gle::Scene* scene, gle::Mesh* mesh);
    void _renderDebugMeshes(gle::Scene* scene);

    gle::Program*	_currentProgram;
    gle::Program*	_shadowMapProgram;
    gle::Bufferui	_indexesBuffer;
    int			_debugMode;
    gle::Program*	_debugProgram;
  };

};

#endif /* _GLE_RENDERER_HPP_ */
