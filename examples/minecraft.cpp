//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Thu Jul 12 00:06:03 2012 loick michard
//

#include <iostream>

#include "TextureFrameBuffer.hpp"
#include "Example.hpp"
#include "CubeMap.hpp"
#include "ObjLoader.hpp"
#include "UniversalLoader.hpp"
#include "Mesh.hpp"
#include "Geometries.hpp"
#include "trackball.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720

#define W_FRAMERATE 60

int accelerometerId = 0;

class App : public Example {
public:
  App(int ac, char** av) :
    Example(ac, av, W_WIDTH, W_HEIGHT, W_FRAMERATE, "glEngine : Minecraft"),
    _light(), _plane()
  {
    _cameraType = Trackball;
    //_recordVideo = true;
  }

  
  void _setCubeTextureCoords(gle::Mesh* cube)
  {
    GLfloat textureCoords[] = {
      // Front face  
      0.0, 0.0,
      1.0/3.0, 0.0,
      1.0/3.0, 1.0,
      0.0, 1.0,
      
      // Back face
      1.0, 0.0,
      1.0, 1.0,
      2.0/3.0, 1.0,
      2.0/3.0, 0.0,
      
      // Top face
      
      2.0/3.0, 1.0,
      2.0/3.0, 0.0,
      1.0/3.0, 0.0,
      1.0/3.0, 1.0,
      
      // Bottom face
      
      1.0/3.0, 0.0,
      2.0/3.0, 0.0,
      2.0/3.0, 1.0,
      1.0/3.0, 1.0,
      
      // Right face
      
      2.0/3.0, 0.0,
      1.0/3.0, 0.0,
      1.0/3.0, 1.0,
      2.0/3.0, 1.0,
      
      // Left face
      
      1.0/3.0, 1.0,
      1.0/3.0, 0.0,
      2.0/3.0, 0.0,
      2.0/3.0, 1.0
    };
    
    cube->setTextureCoords(textureCoords,
			   sizeof(textureCoords) / sizeof(GLfloat));
  }

  void initScene()
  {
    sf::Image* heightmap = new sf::Image();
    heightmap->loadFromFile("./examples/textures/heightmap.png");
    _scene = new gle::Scene();
    _scene->setBackgroundColor(gle::Colorf(1, 1, 1));
    _scene->setFogColor(gle::Colorf(1, 1, 1));
    //_scene->setFogDensity(0.008);
    _scene->enableFrustumCulling();
    gle::CubeMap* cubeMap = new gle::CubeMap("./examples/cubemaps/sky", ".jpg");
    _scene->setEnvMap(cubeMap);
    _camera = new gle::PerspectiveCamera(gle::Vector3f(0, 100, 0),
					 gle::Vector3f(0, 0, 0),
					 45, (GLfloat)_winWidth/_winHeight, 1, 400);
    _renderer = new gle::Renderer();
    //_renderer->setDebugMode(gle::Renderer::Octree);
    _light = new gle::PointLight(gle::Vector3f(0, 0, 0),
    				 gle::Colorf(0.3, 0.3, 0.3));
    gle::PointLight* light = new gle::PointLight(gle::Vector3f(1000, 1000, 0),
						 gle::Colorf(0.8, 0.8, 0.8));
    gle::PointLight* light2 = new gle::PointLight(gle::Vector3f(0, 1000, 1000),
						 gle::Colorf(0.8, 0.8, 0.8));
    gle::Material* cubeMaterial = new gle::Material();
    cubeMaterial->setColorMap(new gle::Texture("./examples/textures/floor.png"));
    cubeMaterial->setNormalMap(new gle::Texture("./examples/textures/floor_normalmap.png"));
    int width = 100;
    int height = 100;
    float size = 4.0;
    int imgX = heightmap->getSize().x - 1;
    int imgY = heightmap->getSize().y - 1;
    std::cout << imgX << " " << imgY << std::endl;
    float maxH = 40.0;
    for (int i = 0; i < width; ++i)
      {
	std::cout << i << "/" << width << "\r";
	std::cout << std::flush;
	for (int j = 0; j < height; ++j)
	  {
	    sf::Color color = heightmap->getPixel((int)(((float)i / width) * imgX) + 1, (int)(((float)j / height) * imgY) + 1);
	    int hh = (int)color.r / (255 / (int)maxH);
	    float height2 = size * hh / 2;
	    gle::Mesh* cube = gle::Geometries::Cube(cubeMaterial, size);
	    cube->setPosition(gle::Vector3f(i * size - (float)height / 2.0 * size, height2, j * size - (float)width / 2.0 * size));
	    _setCubeTextureCoords(cube);
	    *_scene << cube;
	  }
      }
    *_scene << _camera << light << light2;

    _camera->addChild(_light);
  }

  void animate()
  {
    trackball::rotationY += 0.025 / 10;
    trackball::rotationX = 1.168;
    //trackball::zoom = 48.46; 
    trackball::zoom += 0.12 / 10;
  }

  void render()
  {
    _renderer->render(_scene, gle::Rectf(0, 0, _winWidth, _winHeight));
  }

  void  catchEvent(sf::Event& event)
  {
    (void)event;
  }

private	:
  gle::UniversalLoader* _loader;
  gle::CubeMap* _cubeMap;
  gle::PointLight*		_light;
  gle::Mesh*			_plane;
  gle::Mesh*			_cube;
  gle::TextureFrameBuffer*	_framebuffer;
  gle::Material*		_bodyMaterial;
};

int main(int ac, char **av)
{
  try {
    App app(ac, av);
    return (app.run());
  }
  catch (gle::Exception::CompilationError* e)
    {
      std::cout << "Compilation error in " << e->getFilename() << ":\n"
		<< e->what() << std::endl;
    }
  catch (gle::Exception::InvalidOperation* e)
    {
      std::cout << "Invalid Operation: " << e->what() << std::endl;
    }
  catch (std::exception *e)
    {
      std::cout << "Error: " << e->what() << std::endl;
    }
  return (EXIT_FAILURE);
}
