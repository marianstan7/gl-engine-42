//
// main.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 15 17:24:36 2012 gael jochaud-du-plessix
// Last update Mon Apr 30 15:44:46 2012 gael jochaud-du-plessix
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

#include "flycam.hpp"
#include "video.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720

#define W_FRAMERATE 30

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

  sf::Window App(sf::VideoMode(W_WIDTH, W_HEIGHT, 32), "Example 01",
		 sf::Style::Default, context);
  App.setActive();

  gle::Scene			scene;
  gle::PerspectiveCamera	camera(gle::Vector3<GLfloat>(0, 0, 0),
				       gle::Vector3<GLfloat>(0, 30, 0),
				       45, (GLfloat)W_WIDTH/W_HEIGHT,
				       1, 10000);
  gle::Material			material;

  material.setDiffuseLightEnabled(true);
  material.setSpecularLightEnabled(true);
  material.setShininess(32);

  gle::ObjLoader loader;
  gle::Mesh* model = loader.load("./models/WaltDisneyHeads.obj", &material);

  if (model)
    {
      std::vector<gle::Mesh*> childs;
      model->getChildrenByName("DisneyHead_hires", childs);
      model->getChildrenByName("Disney_hires", childs);
      scene << childs;
    }
  else
    exit(-1);

  gle::Material			materialLight;

  materialLight.setDiffuseLightEnabled(false);
  materialLight.setSpecularLightEnabled(false);
  materialLight.setAmbientColor(gle::Color<GLfloat>(1, 0, 0.25));

  gle::PointLight		l(gle::Vector3<GLfloat>(0, 100, 0), gle::Color<GLfloat>(1, 0, 0.25));
  gle::Mesh*			sp = gle::Geometries::Sphere(&materialLight,
							     1);
  gle::PointLight		l2(gle::Vector3<GLfloat>(0, 0, -20),
				   gle::Color<GLfloat>(0, 0.25, 1));
  gle::Material			materialLight2;
  materialLight2.setDiffuseLightEnabled(false);
  materialLight2.setSpecularLightEnabled(false);
  materialLight2.setAmbientColor(gle::Color<GLfloat>(0, 0.25, 1));
  gle::Mesh*			sp2 = gle::Geometries::Sphere(&materialLight2,
							      1);
  gle::Material			materialLight3;
  materialLight3.setDiffuseLightEnabled(false);
  materialLight3.setSpecularLightEnabled(false);
  materialLight3.setAmbientColor(gle::Color<GLfloat>(0.5, 1, 0.5));
  gle::PointLight		l3(gle::Vector3<GLfloat>(40, 0, 0),
				   gle::Color<GLfloat>(0.5, 1, 0.5));
  gle::Mesh*			sp3 = gle::Geometries::Sphere(&materialLight3,
							      1);

  gle::Material			materialLight4;
  materialLight4.setDiffuseLightEnabled(false);
  materialLight4.setSpecularLightEnabled(false);
  materialLight4.setAmbientColor(gle::Color<GLfloat>(1, 0.66, 0));
  gle::PointLight		l4(gle::Vector3<GLfloat>(40, 0, 0),
				   gle::Color<GLfloat>(1, 0.66, 0));
  gle::Mesh*			sp4 = gle::Geometries::Sphere(&materialLight4,
							      1);
  gle::PointLight		l5(gle::Vector3<GLfloat>(40, 0, 0),
				   gle::Color<GLfloat>(0.1, 0.1, 0.1));
  sp->setPosition(gle::Vector3<GLfloat>(0, 100, 0));
  sp2->setPosition(gle::Vector3<GLfloat>(0, 0, -20));
  sp3->setPosition(gle::Vector3<GLfloat>(40, 0, 0));

  //gle::DirectionalLight		l4(gle::Vector3<GLfloat>(1, 1, 0), gle::Color<GLfloat>(0.5, 0.5, 0.5));

  scene << &camera << &material << &materialLight << &materialLight2 << &materialLight3 << &materialLight4 << sp << sp2 << sp3 << sp4;
  scene << &l << &l2 << &l3 << &l4 << &l5;

  gle::Renderer		renderer;
  GLfloat		angle = 0;
  sf::Clock		clock;
  sf::Clock		time;
  size_t		frameCounter = 0;
  float			dist = 170;
  
  while (App.isOpen())
    {
      sf::Event event;
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
	  flycam::event(event, App);
	}
      camera.setPosition(gle::Vector3<GLfloat>(cos(angle/3) * -(dist/2),
					       30,
					       sin(angle/3) * -dist/0.8));
      l5.setPosition(gle::Vector3<GLfloat>(cos(angle/3) * -dist,
					   30,
					   sin(angle/3) * -dist));

      float		dist2 = dist / 3;

      sp->setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, cos(angle/2) * dist2 + 30, 0));
      l.setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, cos(angle/2) * dist2 + 30, 0));

      sp2->setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, 30, cos(angle/2) * dist2));
      l2.setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, 30, cos(angle/2) * dist2));

      sp3->setPosition(gle::Vector3<GLfloat>(0, sin(angle/2) * dist2+30, cos(angle/2) * dist2));
      l3.setPosition(gle::Vector3<GLfloat>(0, sin(angle/2) * dist2+30, cos(angle/2) * dist2));

      sp4->setPosition(gle::Vector3<GLfloat>(0, cos(angle/2) * dist2+30, sin(angle/2) * dist2));
      l4.setPosition(gle::Vector3<GLfloat>(0, cos(angle/2) * dist2+30, sin(angle/2) * dist2));

      scene.updateLights();

      renderer.render(&scene);
      App.display();
      //video::saveImage(App, W_FRAMERATE);
    }

  //video::save(av[0], W_FRAMERATE);
  
  return (0);
}
