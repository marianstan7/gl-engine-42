//
// main.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 15 17:24:36 2012 gael jochaud-du-plessix
// Last update Fri Mar 16 13:55:59 2012 gael jochaud-du-plessix
//

/*! 
  Example using a .obj head and 3 PointLights
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

#define WIDTH 1200.0
#define HEIGHT 800.0

#include "video.hpp"
#define W_FRAMERATE 30

int glEngine(char **av);

int main(int ac, char **av)
{
  int ret;

  try {
    ret = glEngine(av);
  }
  catch (std::exception *e)
    {
      std::cout << "Error: " << e->what() << std::endl;
    }
  return (ret);
}

int glEngine(char **av)
{
  sf::ContextSettings context;
  context.depthBits = 24;
  context.stencilBits = 24;
  context.antialiasingLevel = 2;
  context.majorVersion = 3;
  context.minorVersion = 3;

  sf::Window App(sf::VideoMode(WIDTH, HEIGHT, 32), "Example 02",
		 sf::Style::Default, context);
  App.setActive();

  gle::Scene			scene;
  gle::PerspectiveCamera	camera(gle::Vector3<GLfloat>(0, 500, 1000),
				       gle::Vector3<GLfloat>(0, 0, 0),
				       45, WIDTH / HEIGHT, 1, 10000);

  gle::Mesh solarSystem;

  gle::Material sunMaterial;
  sunMaterial.setDiffuseLightEnabled(true);
  sunMaterial.setSpecularLightEnabled(true);
  sunMaterial.setShininess(32);
  sunMaterial.setAmbientColor(gle::Color<GLfloat>(1, 1, 1));
  sunMaterial.setColorMap(new gle::Texture("./examples/sun.jpg"));

  gle::Mesh *sun = gle::Geometries::Sphere(&sunMaterial, 100, 300, 300);
  solarSystem.addChild(sun);
  gle::Mesh earthSystem;
  gle::Material earthMaterial;
  earthMaterial.setDiffuseLightEnabled(true);
  earthMaterial.setSpecularLightEnabled(true);
  earthMaterial.setShininess(32);
  earthMaterial.setColorMap(new gle::Texture("./examples/earth.jpg"));
  gle::Mesh *earth = gle::Geometries::Sphere(&earthMaterial, 40, 300, 300);
  earthSystem.addChild(earth);
  earth->setPosition(gle::Vector3<GLfloat>(500, 0, 0));
  solarSystem.addChild(&earthSystem);

  gle::Mesh moonSystem;
  gle::Material moonMaterial;
  moonMaterial.setDiffuseLightEnabled(true);
  moonMaterial.setSpecularLightEnabled(true);
  moonMaterial.setShininess(32);
  moonMaterial.setColorMap(new gle::Texture("./examples/moon.jpg"));
  gle::Mesh *moon = gle::Geometries::Sphere(&moonMaterial, 7, 300, 300);
  moonSystem.addChild(moon);
  earth->addChild(&moonSystem);
  moon->setPosition(gle::Vector3<GLfloat>(100, 0, 0));
  gle::PointLight sunLight(gle::Vector3<GLfloat>(0, 0, 0),
			   gle::Color<GLfloat>(1, 1, 0.8),
			   gle::Color<GLfloat>(1, 1, 0.8));
  scene << sun << &sunMaterial << &camera << earth << &earthMaterial << &sunLight;
  scene << &moonMaterial << moon;

  gle::Renderer		renderer;
  GLfloat		angle = 0;
  sf::Clock		clock;
  sf::Clock		time;
  size_t		frameCounter = 0;
  
  renderer.createPrograms(&scene);
  while (App.isOpen())
    {
      if (time.getElapsedTime().asMilliseconds() >= 100)
	{
	  std::cout << "fps:" << (frameCounter * 10) << std::endl;
	  frameCounter = 0;
	  time.restart();
	}
      frameCounter++;
      angle = (float)clock.getElapsedTime().asMilliseconds() / 1000;
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
	}
      earthSystem.setRotation(gle::Vector3<GLfloat>(0, 1, 0), angle * 10);
      earth->setRotation(gle::Vector3<GLfloat>(0, 1, 0), angle * 100);
      sun->setRotation(gle::Vector3<GLfloat>(0, 1, 0), angle * 10);
      renderer.render(&scene);
      App.display();
      GLfloat elapsed = time.getElapsedTime().asMicroseconds();
      if (1000000/W_FRAMERATE - elapsed > 0)
        {
	  sf::sleep(sf::microseconds(1000000/W_FRAMERATE - elapsed));
          time.restart();
        }
      video::saveImage(App, W_FRAMERATE);

    }
  video::save(av[0], W_FRAMERATE);
  
  return (0);
}
