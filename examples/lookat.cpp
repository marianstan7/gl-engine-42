//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Wed Jun  6 19:53:25 2012 gael jochaud-du-plessix
//

#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <Scene.hpp>
#include <PerspectiveCamera.hpp>
#include <Geometries.hpp>
#include <Material.hpp>
#include <Renderer.hpp>
#include <ObjLoader.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <MeshBufferManager.hpp>

#include "Example.hpp"
#include "flycam.hpp"
#include "fps.hpp"
#include "video.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720

#define W_FRAMERATE 60

int accelerometerId = 0;

class App : public Example {
public:
  App(int ac, char** av) :
    Example(ac, av, W_WIDTH, W_HEIGHT, W_FRAMERATE, "glEngine : LookAt"),
    _light()
  {
    _cameraType = Trackball;
  }

  void initScene()
  {
    _scene = new gle::Scene();
    _camera = new gle::PerspectiveCamera(gle::Vector3f(0, 0, 200),
					 gle::Vector3f(0, 0, 0),
					 45, (GLfloat)_winWidth/_winHeight, 1, 10000);
    _renderer = new gle::Renderer();

    gle::Material* material = new gle::Material();
    material->setDiffuseLightEnabled(true);
    material->setSpecularLightEnabled(true);

    gle::Material* materialLight = new gle::Material();
    materialLight->setDiffuseLightEnabled(true);
    materialLight->setSpecularLightEnabled(true);
    materialLight->setAmbientColor(gle::Color<GLfloat>(1, 1, 1));
    materialLight->setSpecularColor(gle::Color<GLfloat>(1, 1, 1));
    materialLight->setDiffuseColor(gle::Color<GLfloat>(1, 1, 1));

    _light = new gle::PointLight(gle::Vector3f(0, 0, 0),
				 gle::Colorf(1.0, 1.0, 1.0));
    _cameraLight = new gle::PointLight(gle::Vector3f(0, 0, 0),
				       gle::Colorf(0.4, 0.4, 0.4));
    sp = gle::Geometries::Sphere(materialLight, 5, 100, 100);
    sp->setPosition(gle::Vector3f(0, 0, 0));
    sp->addChild(_light);
    _camera->addChild(_cameraLight);

    *_scene << _camera << sp;// << _cameraLight;
    
    for (GLfloat i = -70.0; i <= 70.0; i += 15.0)
      for (GLfloat j = -70.0; j <= 70.0; j += 15.0)
	for (GLfloat k = -70.0; k <= 70.0; k += 15.0)
	  {
	    gle::Material* materialTriangle = new gle::Material();
	    materialTriangle->setDiffuseLightEnabled(true);
	    materialTriangle->setSpecularLightEnabled(true);
	    materialTriangle->setDiffuseColor(gle::Color<GLfloat>((float)rand() / float(RAND_MAX),
								  (float)rand() / float(RAND_MAX),
								  (float)rand() / float(RAND_MAX)));
	    gle::Mesh* triangle = gle::Geometries::Cuboid(materialTriangle, 3, 3, 11);
	    triangle->setPosition(gle::Vector3f(i, j, k));
	    *_scene << triangle;
	    _triangles.push_back(triangle);
	  }

    _scene->update();
  }

  void animate()
  {
    static double t = 0.0;
    sp->setPosition(gle::Vector3f(sin(t * 0.7) * 70,
				  cos(t * 0.5) * 70,
				  cos(t * 0.3) * 70));
    for (gle::Mesh* &triangle : _triangles)
      triangle->setTarget(sp->getAbsolutePosition());
    //_cameraLight->setPosition(_camera->getAbsolutePosition());
    _scene->updateLights();
    _camera->setTarget(sp->getAbsolutePosition());
    t += 0.01;
  }

private:
  gle::PointLight*	_light;
  gle::PointLight*	_cameraLight;
  std::vector<gle::Mesh*> _triangles;
  gle::Mesh*		sp;
};

int main(int ac, char **av)
{
  try {
    App app(ac, av);
    return (app.run());
  }
  catch (std::exception *e)
    {
      std::cout << "Error: " << e->what() << std::endl;
    }
  return (EXIT_FAILURE);
}
