//
// main.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 15 17:24:36 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 22:21:09 2012 loick michard
//

/*! \mainpage glEngine Reference documentation
  
  \section intro_sec Introduction

  glEngine is a 3D engine that use OpenGL 4.2 for rendering.<br>
  bla, bla, bla...

 */

#include <iostream>
#include <SFML/Window.hpp>
#include <Scene.hpp>
#include <PerspectiveCamera.hpp>
#include <Geometries.hpp>
#include <Material.hpp>
#include <Renderer.hpp>
#include <ObjLoader.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <Exception.hpp>

#include "Example.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720

#define W_FRAMERATE 60

class App : public Example {
public:
  App(int ac, char** av) :
    Example(ac, av, W_WIDTH, W_HEIGHT, W_FRAMERATE, "glEngine : Wavefront OBJ loader"),
    _light()
  {
    _cameraType = Trackball;
  }

  void initScene()
  {
    _scene = new gle::Scene();
    _camera = new gle::PerspectiveCamera(gle::Vector3f(-20, 10, 0),
                                         gle::Vector3f(0, 0, 0),
                                         45, (GLfloat)_winWidth/_winHeight, 1, 10000);

    _renderer = new gle::Renderer();

    gle::Material* material = new gle::Material();
    
    material->setDiffuseLightEnabled(true);
    material->setSpecularLightEnabled(true);
  
    material->setShininess(32);

    gle::ObjLoader loader;
    gle::Scene::Node* model = loader.load(_argv[1], material);

    if (!model)
      throw new gle::Exception::Exception("Cannot load model " + _argv[1]);

    *_scene << model;
    *_scene << _camera;

    _light = new gle::PointLight(gle::Vector3f(0, 2000, 0),
				 gle::Colorf(0.8, 0.8, 0.8));

    *_scene << _light;

  }

  void animate()
  {
    _light->setPosition(_camera->getAbsolutePosition());
  }

private:
  gle::PointLight*      _light;
};

int main(int ac, char **av)
{
  try {
    App app(ac, av);
    return (app.run());
  }
  catch (gle::Exception::ParsingError* error)
    {
      std::cout << error->getFilename() << ":" << error->getLine() << " " << error->what() << std::endl;
    }
  catch (std::exception *e)
    {
      std::cout << "Error: " << e->what() << std::endl;
    }
  return (EXIT_FAILURE);
}
