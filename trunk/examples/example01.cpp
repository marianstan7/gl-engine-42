//
// main.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 15 17:24:36 2012 gael jochaud-du-plessix
// Last update Thu Mar  1 18:34:15 2012 gael jochaud-du-plessix
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
  context.DepthBits = 24;
  context.StencilBits = 24;
  context.AntialiasingLevel = 2;
  context.MajorVersion = 3;
  context.MinorVersion = 3;

  sf::Window App(sf::VideoMode(800, 800, 32), "Example 01",
		 sf::Style::Default, context);
  App.SetActive();

  gle::Scene			scene;
  gle::PerspectiveCamera	camera(gle::Vector3<GLfloat>(0, 0, 0),
				       gle::Vector3<GLfloat>(0, 30, 0),
				       45, 1, 1, 10000);
  gle::Material			material;

  material.setDiffuseLightEnabled(true);
  material.setSpecularLightEnabled(true);
  material.setShininess(32);

  gle::ObjLoader loader;
  gle::Mesh* model = loader.load("../models/WaltDisneyHeads.obj", &material);

  if (model)
    {
      std::vector<gle::Mesh*> childs = model->getChildren();
      for (std::vector<gle::Mesh*>::iterator it = childs.begin();
	   it != childs.end(); ++it)
	{
	  gle::Mesh* mesh = *it;
	  if (mesh->getName() == "DisneyHead_hires" ||
	      mesh->getName() == "Disney_hires")
	    {
	      scene << mesh;
	    }
	}
    }
  else
    exit(-1);

  gle::Material			materialLight;

  materialLight.setDiffuseLightEnabled(false);
  materialLight.setSpecularLightEnabled(false);

  gle::PointLight		l(gle::Vector3<GLfloat>(0, 20, 0), gle::Color<GLfloat>(0.8, 0, 0));
  gle::Mesh*			sp = gle::Geometries::Sphere(&materialLight, 1, gle::Color<GLfloat>(0.8, 0, 0));
  gle::PointLight		l2(gle::Vector3<GLfloat>(0, 0, -20), gle::Color<GLfloat>(0, 0, 0.8));
  gle::Mesh*			sp2 = gle::Geometries::Sphere(&materialLight, 1, gle::Color<GLfloat>(0, 0, 0.8));
  gle::PointLight		l3(gle::Vector3<GLfloat>(40, 0, 0), gle::Color<GLfloat>(0, 0.8, 0));
  gle::Mesh*			sp3 = gle::Geometries::Sphere(&materialLight, 1, gle::Color<GLfloat>(0, 0.8, 0));

  sp->setPosition(gle::Vector3<GLfloat>(0, 20, 0));
  sp2->setPosition(gle::Vector3<GLfloat>(0, 0, -20));
  sp3->setPosition(gle::Vector3<GLfloat>(40, 0, 0));

  gle::DirectionalLight		l4(gle::Vector3<GLfloat>(1, 1, 0), gle::Color<GLfloat>(0.1, 0.1, 0.1));

  scene << &camera << &material << &materialLight << sp << sp2 << sp3;
  scene << &l << &l2 << &l3 << &l4;
  scene.setLightEnabled(true, gle::Color<GLfloat>(0, 0, 0));

  gle::Renderer		renderer;
  GLfloat		angle = 0;
  sf::Clock		clock;
  sf::Clock		time;
  size_t		frameCounter = 0;
  float			dist = 170;
  
  renderer.createPrograms(&scene);
  while (App.IsOpen())
    {
      if (time.GetElapsedTime().AsMilliseconds() >= 100)
	{
	  std::cout << "fps:" << (frameCounter * 10) << std::endl;
	  frameCounter = 0;
	  time.Restart();
	}
      frameCounter++;
      angle = (float)clock.GetElapsedTime().AsMilliseconds() / 1000;
      sf::Event Event;
      while (App.PollEvent(Event))
	{
	  // Close window : exit
	  if (Event.Type == sf::Event::Closed)
	    App.Close();
	  else if (Event.Type == sf::Event::KeyPressed
		   && Event.Key.Code == sf::Keyboard::Escape)
	    App.Close();
	  // Adjust the viewport when the window is resized
	  if (Event.Type == sf::Event::Resized)
	    glViewport(0, 0, Event.Size.Width, Event.Size.Height);
	}
      camera.setPosition(gle::Vector3<GLfloat>(cos(angle/3) * -dist,
					       30,
					       sin(angle/3) * -dist));

      float		dist2 = dist / 3;

      sp->setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, cos(angle/2) * dist2 + 30, 0));
      l.setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, cos(angle/2) * dist2 + 30, 0));

      sp2->setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, 30, cos(angle/2) * dist2));
      l2.setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, 30, cos(angle/2) * dist2));

      sp3->setPosition(gle::Vector3<GLfloat>(0, sin(angle/2) * dist2+30, cos(angle/2) * dist2));
      l3.setPosition(gle::Vector3<GLfloat>(0, sin(angle/2) * dist2+30, cos(angle/2) * dist2));

      scene.updateLights();

      renderer.render(&scene);
      App.Display();
    }
  
  return (0);
}
