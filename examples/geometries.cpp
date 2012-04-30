//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Mon Apr 30 15:16:29 2012 gael jochaud-du-plessix
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

#include "flycam.hpp"
#include "video.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720

#define W_FRAMERATE 30

int accelerometerId = 0;

int glEngine(int, char**);

int main(int ac, char **av)
{
  int ret;

  try {
    ret = glEngine(ac, av);
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
  gle::PerspectiveCamera camera(gle::Vector3<GLfloat>(-20, 10, 0),
				gle::Vector3<GLfloat>(0, 0, 0),
				45, (GLfloat)W_WIDTH/W_HEIGHT, 1, 10000);

  gle::ObjLoader loader;
  gle::Material material;
  material.setDiffuseLightEnabled(true);
  material.setSpecularLightEnabled(true);
  material.setColorMap(new gle::Texture("./examples/earth.jpg"));

  gle::Material material2;
  material2.setDiffuseLightEnabled(true);
  material2.setSpecularLightEnabled(true);

  gle::Mesh* cube = gle::Geometries::Cube(&material, 10);
  cube->setPosition(gle::Vector3<GLfloat>(8, 0, -8));
  gle::Mesh* pointCube = gle::Geometries::Cube(&material2, 10);
  pointCube->setPosition(gle::Vector3<GLfloat>(-24, 0, -8));
  pointCube->setType(gle::Mesh::Points);
  pointCube->setPointSize(5);
  gle::Mesh* sphere = gle::Geometries::Sphere(&material, 5);
  sphere->setPosition(gle::Vector3<GLfloat>(8, 0, 8));
  gle::Mesh* pointSphere = gle::Geometries::Sphere(&material2, 5, 100, 100);
  pointSphere->setPosition(gle::Vector3<GLfloat>(-24, 0, 8));
  pointSphere->setType(gle::Mesh::Points);
  gle::Mesh* plane = gle::Geometries::Plane(&material, 80, 80, 2, 2);
  plane->setPosition(gle::Vector3<GLfloat>(0, -5, 0));
  gle::Mesh* pointPlane =
    gle::Geometries::Plane(&material2, 80, 80, 100, 100);
  pointPlane->setPosition(gle::Vector3<GLfloat>(0, -5, 80));
  pointPlane->setType(gle::Mesh::Points);
  pointPlane->setPointSize(2);

  gle::Mesh* wiredSphere = new gle::Mesh(*sphere);
  wiredSphere->setPosition(gle::Vector3<GLfloat>(-8, 0, 8));
  wiredSphere->setRasterizationMode(gle::Mesh::Line);
  wiredSphere->setMaterial(&material2);

  gle::Mesh* wiredCube = new gle::Mesh(*cube);
  wiredCube->setPosition(gle::Vector3<GLfloat>(-8, 0, -8));
  wiredCube->setRasterizationMode(gle::Mesh::Line);
  wiredCube->setMaterial(&material2);

  gle::Mesh* wiredPlane = new gle::Mesh(*plane);
  wiredPlane->setPosition(gle::Vector3<GLfloat>(0, -5, -80));
  wiredPlane->setRasterizationMode(gle::Mesh::Line);
  wiredPlane->setMaterial(&material2);

  gle::Material materialShadow;
  materialShadow.setDiffuseLightEnabled(true);
  materialShadow.setSpecularLightEnabled(true);
  materialShadow.setColorMap(new gle::Texture("./examples/shadow.png"));

  gle::Mesh* shadow = gle::Geometries::Plane(&materialShadow, 80, 80, 2, 2);
  shadow->setPosition(gle::Vector3<GLfloat>(0, -4, 0));

  scene << cube << wiredCube << pointCube;
  scene << sphere << wiredSphere << pointSphere;
  scene << plane << wiredPlane << pointPlane;// << shadow;

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

  gle::Renderer renderer;

  sf::Clock clock;
  sf::Clock time;
  sf::Clock frameTimer;
  int frameCounter = 0;

  App.setMouseCursorVisible(false);
  
  while (App.isOpen())
    {    
      if (frameTimer.getElapsedTime().asMilliseconds() >= 1000)
        {
	  std::cout << "fps:" << (frameCounter * 1) << std::endl;
          frameCounter = 0;
          frameTimer.restart();
        }
      frameCounter++;
  
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
      l.setPosition(camera.getPosition());
      scene.updateLights();
      renderer.render(&scene);
      App.display();
      plane->setRotation(gle::Vector3<GLfloat>(1, 0, 0),
			 -sf::Joystick::getAxisPosition(accelerometerId,
							sf::Joystick::X));
      GLfloat elapsed = time.getElapsedTime().asMicroseconds();
      if (1000000/W_FRAMERATE - elapsed > 0)
	{
	  sf::sleep(sf::microseconds(1000000/W_FRAMERATE - elapsed));
	  time.restart();
	}
      //video::saveImage(App, W_FRAMERATE);
    }

  //video::save(av[0], W_FRAMERATE);

  return (0);
}
