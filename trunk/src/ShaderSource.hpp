//
// ShaderSource.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 21 15:47:17 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 19:51:33 2012 gael jochaud-du-plessix
//

#ifndef _GLE_SHADER_SOURCE_HPP_
# define _GLE_SHADER_SOURCE_HPP_

#include <gle/opengl.h>

namespace gle {

  namespace ShaderSource {

    //! Source of the vertex ubber shader
    extern const char *VertexShader;

    //! Source of the fragment ubber shader
    extern const char *FragmentShader;

    //! Source of the cube map vertex shader
    extern const char *CubeMapVertexShader;

    //! Source of the cube map fragment shader
    extern const char *CubeMapFragmentShader;

    //! Source of the debug vertex shader
    extern const char *DebugVertexShader;

    //! Source of the debug fragment shader
    extern const char *DebugFragmentShader;

    //! Source of the shadow map vertex shader
    extern const char *ShadowMapVertexShader;
    
    //! Source of the shadow map fragment shader
    extern const char *ShadowMapFragmentShader;

    //! Attribute location of the vertex position
    extern GLuint PositionLocation;

    //! Attribute location of the vertex normal
    extern GLuint NormalLocation;

    //! Attribute location of the vertex tangent
    extern GLuint TangentLocation;

    //! Attribute location of the vertex texture coords
    extern GLuint TextureCoordLocation;

    //! Attribute location of the vertex bones
    extern GLuint BonesLocation;

    //! Attribute location of the vertex mesh identifier
    extern GLuint MeshIdentifierLocation;
  }
}

#endif /* _GLE_SHADER_SOURCE_HPP_ */
