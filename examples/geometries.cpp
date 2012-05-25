//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Fri May 25 13:56:24 2012 gael jochaud-du-plessix
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
  App() :
    Example(W_WIDTH, W_HEIGHT, W_FRAMERATE, "glEngine : Geometries"),
    _light(), _plane()
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
    material->setColorMap(new gle::Texture("./examples/earth.jpg"));

    gle::Material* material2 = new gle::Material();
    material2->setDiffuseLightEnabled(true);
    material2->setSpecularLightEnabled(true);

    gle::Mesh* cube = gle::Geometries::Cube(material, 10);
    cube->setPosition(gle::Vector3f(8, 0, -8));
    cube->setRotation(gle::Vector3f(0, 1, 0), 45);
    gle::Mesh* pointCube = gle::Geometries::Cube(material2, 10);
    pointCube->setPosition(gle::Vector3f(-24, 0, -8));
    pointCube->setPrimitiveType(gle::Mesh::Points);
    pointCube->setPointSize(5);
    gle::Mesh* sphere = gle::Geometries::Sphere(material, 5);
    sphere->setPosition(gle::Vector3f(8, 0, 8));
    gle::Mesh* pointSphere = gle::Geometries::Sphere(material2, 5, 100, 100);
    pointSphere->setPosition(gle::Vector3f(-24, 0, 8));
    pointSphere->setPrimitiveType(gle::Mesh::Points);
    pointSphere->setScale(3);
    _plane = gle::Geometries::Plane(material, 80, 80, 20, 20);
    _plane->setPosition(gle::Vector3f(0, -5, 0));
    gle::Mesh* pointPlane =
      gle::Geometries::Plane(material2, 80, 80, 100, 100);
    pointPlane->setPosition(gle::Vector3f(0, -5, 80));
    pointPlane->setPrimitiveType(gle::Mesh::Points);
    pointPlane->setPointSize(2);

    gle::Mesh* wiredSphere = new gle::Mesh(*sphere);
    wiredSphere->setPosition(gle::Vector3f(-8, 0, 8));
    wiredSphere->setRasterizationMode(gle::Mesh::Line);
    wiredSphere->setMaterial(material2);

    gle::Mesh* wiredCube = new gle::Mesh(*cube);
    wiredCube->setPosition(gle::Vector3f(-8, 0, -8));
    wiredCube->setRasterizationMode(gle::Mesh::Line);
    wiredCube->setMaterial(material2);

    gle::Mesh* wiredPlane = new gle::Mesh(*_plane);
    wiredPlane->setPosition(gle::Vector3f(0, -5, -80));
    wiredPlane->setRasterizationMode(gle::Mesh::Line);
    wiredPlane->setMaterial(material2);

    *_scene << cube << wiredCube << pointCube;
    *_scene << sphere << wiredSphere << pointSphere;
    *_scene << _plane << wiredPlane << pointPlane;

    gle::Material* materialLight = new gle::Material();

    materialLight->setDiffuseLightEnabled(true);
    materialLight->setSpecularLightEnabled(true);
    _light = new gle::PointLight(gle::Vector3f(0, 2000, 0),
				 gle::Colorf(0.8, 0.8, 0.8));
    gle::Mesh* sp = gle::Geometries::Sphere(materialLight, 10);
    sp->setPosition(gle::Vector3f(0, 200, 0));

    *_scene << _camera << sp;
    *_scene << _light;

    _scene->updateScene();
  }

  void animate()
  {
    _light->setPosition(_camera->getAbsolutePosition());
    _scene->updateLights();
    _plane->setRotation(gle::Vector3<GLfloat>(1, 0, 0),
    			-sf::Joystick::getAxisPosition(accelerometerId,
    						       sf::Joystick::X));
  }

private:
  gle::PointLight*	_light;
  gle::Mesh*		_plane;
};

int main(int ac, char **av)
{
  int ret;

  try {
    App app;
    return (app.run());
  }
  catch (std::exception *e)
    {
      std::cout << "Error: " << e->what() << std::endl;
    }
  return (ret);
}
