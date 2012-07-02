//
// ShaderSource.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 21 15:47:17 2012 gael jochaud-du-plessix
// Last update Tue Jun 26 19:44:54 2012 gael jochaud-du-plessix
//

#ifndef _GLE_SHADER_SOURCE_HPP_
# define _GLE_SHADER_SOURCE_HPP_

#include <gle/opengl.h>

namespace gle {

  namespace ShaderSource {

    extern const char *VertexShader;
    extern const char *FragmentShader;
    extern const char *CubeMapVertexShader;
    extern const char *CubeMapFragmentShader;
    extern const char *DebugVertexShader;
    extern const char *DebugFragmentShader;
    extern const char *ShadowMapVertexShader;
    extern const char *ShadowMapFragmentShader;
    
    extern GLuint PositionLocation;
    extern GLuint NormalLocation;
    extern GLuint TangentLocation;
    extern GLuint TextureCoordLocation;
    extern GLuint BonesLocation;
    extern GLuint MeshIdentifierLocation;
  }
}

#endif /* _GLE_SHADER_SOURCE_HPP_ */
