//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Fri May 25 15:37:08 2012 loick michard
//

#include <iostream>
#include <cstdio>
#include <map>
#include <SFML/Window.hpp>
#include <Scene.hpp>
#include <OrthographicCamera.hpp>
#include <Geometries.hpp>
#include <Material.hpp>
#include <Renderer.hpp>
#include <ObjLoader.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>

#include "flycam.hpp"
#include "fps.hpp"
#include "video.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720
#define W_FRAMERATE 3000

#define NB_HOUSES 100

int glEngine(int, char**);

int main(int ac, char **av)
{
  int ret;

  try {
    ret = glEngine(ac, av);
  }
  catch (gle::Exception::ParsingError* e)
    {
      std::cout << e->getLine() << ":" << e->what() << std::endl;
    }
  catch (std::exception *e)
    {
      std::cout << "Error: " << e->what() << std::endl;
    }
  return (ret);
}

int glEngine(int ac, char **av)
{
  (void)ac;
  (void)av;
  sf::ContextSettings context;
  context.depthBits = 24;
  context.stencilBits = 24;
  context.antialiasingLevel = 2;
  context.majorVersion = 3;
  context.minorVersion = 3;

  sf::Window App(sf::VideoMode(W_WIDTH, W_HEIGHT, 32), "glEngine",
		 sf::Style::Default, context);

  //! Print OpenGL supported version
  context = App.getSettings();
  std::cout << context.majorVersion << '.'
	    << context.minorVersion << std::endl;

  App.setActive();

  gle::Scene scene;
  //scene.displayBoundingVolume();
  scene.setBackgroundColor(gle::Color<GLfloat>(1.0, 1.0, 1.0));
  gle::OrthographicCamera camera(gle::Vector3<GLfloat>(-50, 20, 0),
				gle::Vector3<GLfloat>(0, 0, 0));

  gle::ObjLoader loader;
  gle::Material material;
  material.setDiffuseLightEnabled(true);
  material.setSpecularLightEnabled(true);

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
      scene << house << house2 << car << car2;
    }

  srand(time(NULL));

  gle::Material materialLight;

  materialLight.setDiffuseLightEnabled(true);
  materialLight.setSpecularLightEnabled(true);
  gle::PointLight l(gle::Vector3<GLfloat>(0, 2000, 0),
		    gle::Color<GLfloat>(0.8, 0.8, 0.8));
  gle::Mesh* sp = gle::Geometries::Sphere(&materialLight, 10);
  sp->setPosition(gle::Vector3<GLfloat>(0, 200, 0));

  scene << &camera << sp;
  scene << &l;
  //camera.addChild(&l);
  gle::Renderer renderer;

  App.setMouseCursorVisible(false);

  double cameraCircleAngle = 0;

  //scene.generateTree();
  //scene.displayTree();
  scene.setCamera(&camera);
  scene.enableFrustumCulling();
  scene.updateScene();
  while (App.isOpen())
    {
      fps::print();

      sf::Event Event;
      while (App.pollEvent(Event))
	{
	  // Close window : exit
	  if (Event.type == sf::Event::Closed)
	    App.close();
	  else if (Event.type == sf::Event::KeyPressed
		   && Event.key.code == sf::Keyboard::Escape)
	    App.close();
	  // Adjust the viewport when the window is resized
	  if (Event.type == sf::Event::Resized)
	    glViewport(0, 0, Event.size.width, Event.size.height);
	  flycam::event(Event, App);
	}
      sf::Mouse::setPosition(sf::Vector2i(W_WIDTH/2, W_HEIGHT/2), App);
      flycam::flycam(&camera);
      /*camera.setPosition(gle::Vector3<GLfloat>(cos(cameraCircleAngle) * 100
	+ 305,
	50,
	sin(cameraCircleAngle) * 100
	+ 75));
	camera.setTarget(gle::Vector3<GLfloat>(305, 15, 75));*/
      l.setPosition(camera.getAbsolutePosition());
      // std::cout << camera.getAbsolutePosition() << std::endl;
      scene.updateLights();
      renderer.render(&scene);
      App.display();
      //video::saveImage(App, W_FRAMERATE);
      fps::limit(W_FRAMERATE);
    }
  
  //video::save(av[0], W_FRAMERATE);
  return (0);
}
