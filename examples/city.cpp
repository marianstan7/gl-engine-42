//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Wed Jun  6 19:53:42 2012 gael jochaud-du-plessix
//

#include <iostream>
#include <cstdio>
#include <map>
#include <SFML/Window.hpp>
#include <Scene.hpp>
#include <PerspectiveCamera.hpp>
#include <Geometries.hpp>
#include <Material.hpp>
#include <Renderer.hpp>
#include <ObjLoader.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>

#include "Example.hpp"
#include "flycam.hpp"
#include "fps.hpp"
#include "video.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720
#define W_FRAMERATE 3000

#define NB_HOUSES 100

class App : public Example {
public:
  App(int ac, char** av) :
    Example(ac, av, W_WIDTH, W_HEIGHT, W_FRAMERATE, "glEngine : City")
  {
    //_cameraType = Trackball;
  }

  void initScene()
  {
    _scene = new gle::Scene();
    _camera = new gle::PerspectiveCamera(gle::Vector3f(-20, 10, 0),
                                         gle::Vector3f(0, 0, 0),
                                         45, (GLfloat)_winWidth/_winHeight, 1, 600);
    _renderer = new gle::Renderer();

    //_scene->displayBoundingVolume();
    _scene->setBackgroundColor(gle::Colorf(1.0, 1.0, 1.0));
    _scene->setFogColor(gle::Colorf(1.0, 1.0, 1.0));
    _scene->setFogDensity(0.004);

    gle::ObjLoader loader;
    gle::Material *material = new gle::Material();
    material->setDiffuseLightEnabled(true);
    material->setSpecularLightEnabled(true);

    sf::Clock modelTime;

    gle::Mesh* houseModel = loader.load("./examples/city_resources/houseUK.obj",
					NULL);
    gle::Mesh* house2Model = new gle::Mesh(*houseModel);
    house2Model->setScale(12, 10, -10);
    gle::Mesh* carModelBase =
      loader.load("./examples/city_resources/db9/db9.obj", NULL);  
    carModelBase->setScale(5, 5, 5);
    carModelBase->setRotation(gle::Vector3<GLfloat>(1, 0, 0), -90);
    gle::Mesh* carModel = new gle::Mesh();
    carModel->addChild(carModelBase);

    std::cout << "Models loading: "
	      << ((float)modelTime.getElapsedTime().asMilliseconds() / 1000)
	      << "s\n";

    // Place the houses
    houseModel->setScale(12, 10, 10);

    for (int i = 0; i < NB_HOUSES; ++i)
      {
	gle::Mesh* house = new gle::Mesh(*houseModel);
	gle::Mesh* house2 = new gle::Mesh(*house2Model);
	gle::Mesh* car = new gle::Mesh(*carModel);
	gle::Mesh* car2 = new gle::Mesh(*carModel);
	car->setPosition(gle::Vector3<GLfloat>(-22 + i * 65, 0.6, 25));
	house->setPosition(gle::Vector3<GLfloat>(i * 71, 0, 0));
	house2->setPosition(gle::Vector3<GLfloat>(i * 71, -0.1, 150));
	car2->setPosition(gle::Vector3<GLfloat>(-22 + i * 65, 0.6, 125));
	*_scene << house << house2 << car << car2;
    }

    gle::Material *materialLight = new gle::Material();

    materialLight->setDiffuseLightEnabled(true);
    materialLight->setSpecularLightEnabled(true);
    gle::PointLight *light = new gle::PointLight(gle::Vector3<GLfloat>(0, 0, 0),
						 gle::Color<GLfloat>(1.0, 1.0, 1.0));
    light->setAttenuation(0, 0, 0.0006);
    _camera->addChild(light);

    *_scene << _camera;

    _scene->enableFrustumCulling();
    _scene->update();
  }

  void animate()
  {
    
  }

private:
  
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
