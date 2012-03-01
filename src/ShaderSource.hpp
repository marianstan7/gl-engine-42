//
// ShaderSource.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 21 15:47:17 2012 gael jochaud-du-plessix
// Last update Thu Mar  1 19:41:07 2012 gael jochaud-du-plessix
//

#ifndef _GLE_SHADER_SOURCE_HPP_
# define _GLE_SHADER_SOURCE_HPP_

namespace gle {

  namespace ShaderSource {
   
    //! Vertex shader sources
    namespace Vertex {

      extern const char *BodyBegin;
      extern const char *BodyEnd;

      //! Default features

      namespace Default {

	extern const char *Head;
	extern const char *UniformDeclarations;	
	extern GLuint PositionLocation;
	extern GLuint NormalLocation;
	extern const char * InputLocations;
	extern const char *InputDeclarations;	
	extern const char *OutputLocations;	
	extern const char *OutputDeclarations;
	extern const char *Body;

      }
      
      //! Light features

      namespace Light {
	extern const char *Head;
	extern const char *UniformDeclarations;
        extern const char *InputLocations;
        extern const char *InputDeclarations;
        extern const char *OutputLocations;
        extern const char *OutputDeclarations;
        extern const char *Body;
      }

      //! Color map features

      namespace ColorMap {
	extern GLuint TextureCoordLocation;
	extern const char * InputLocations;
	extern const char * InputDeclarations;
	extern const char * OutputDeclarations;
	extern const char * Body;
      }

    }
    
    //! Fragment shader sources
    namespace Fragment {
      
      extern const char *BodyBegin;
      extern const char *BodyEnd;

      //! Default features

      namespace Default {
	
	extern const char *Head;
	extern const char * UniformDeclarations;
	extern const char * InputLocations;
	extern const char *InputDeclarations;
	extern const char *OutputLocations;
	extern const char *OutputDeclarations;
	extern const char *Body;

      }

      //! Light features

      namespace Light {
	
	extern const char *InputDeclarations;
	extern const char *Body;

      }

      //! Color map features

      namespace ColorMap {

	extern const char * UniformDeclarations;
	extern const char * InputDeclarations;
	extern const char * Body;
	
      }

    }

  }

}

#endif /* _GLE_SHADER_SOURCE_HPP_ */
