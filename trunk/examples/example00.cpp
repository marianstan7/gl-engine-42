//
// main.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 15 17:24:36 2012 gael jochaud-du-plessix
// Last update Sun May 20 16:17:51 2012 loick michard
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

int glEngine(int, char**);

int main(int ac, char **av)
{
  int ret;

  try {
    ret = glEngine(ac, av);
  }
  catch (gle::Exception::ParsingError* error)
    {
      std::cout << error->getFilename() << ":" << error->getLine() << " " << error->what() << std::endl;
    }
  catch (std::exception *e)
    {
      std::cout << "Error: " << e->what() << std::endl;
    }
  return (ret);
}

int glEngine(int ac, char **av)
{
  if (ac < 3)
    return (0);
  sf::ContextSettings context;
  context.depthBits = 24;
  context.stencilBits = 24;
  context.antialiasingLevel = 2;
  context.majorVersion = 3;
  context.minorVersion = 3;

  sf::Window App(sf::VideoMode(500, 500, 32), "glEngine",
		 sf::Style::Default, context);
  
  //! Print OpenGL supported version
  context = App.getSettings();
  std::cout << context.majorVersion << '.'
	    << context.minorVersion << std::endl;

  App.setActive();

  gle::Scene scene;
  gle::PerspectiveCamera camera(gle::Vector3<GLfloat>(0, 0, 0),
				gle::Vector3<GLfloat>(0, 0, 0),
				45, 1, 1, 1000000);
  gle::Material material;

  material.setDiffuseLightEnabled(true);
  material.setSpecularLightEnabled(true);

  material.setShininess(32);

  gle::ObjLoader loader;
  gle::Mesh* model = loader.load(av[1], &material);

  GLfloat dist = atol(av[2]);

  gle::Mesh* helice = NULL;

  srand(time(NULL));

  if (model)
    {
      scene << model;
    }

  // float nbCubes = 10;
  //  for (float i = 0; i < nbCubes; i++)
  //   {
  //     for (float j = 0; j < nbCubes; j++)
  // 	{
  // 	  for (float k = 0; k < nbCubes; k++)
  // 	    {
  // 	      gle::Mesh* cube = gle::Geometries::Cube(&material,
  // 						      10.0 / nbCubes);
  // 	      cube
  // 		->translate(gle::Vector3<GLfloat>(((i / nbCubes) * 12) - 6.0,
  // 						  ((k / nbCubes) * 12) - 6.0,
  // 						  ((j / nbCubes) * 12) - 6.0)
  // 			    );
  // 	      gle::Color<GLfloat> color((double)random() / RAND_MAX,
  // 					(double)random() / RAND_MAX,
  // 					(double)random() / RAND_MAX, 1.0);
  // 	      gle::VertexAttributeArray<GLfloat> colors(24 * 4);
  // 	      for (int i = 0; i < 24; i++)
  // 		colors.push(color);
  // 	      cube->setColors((GLfloat*)colors, colors.getSize());
  // 	      cube->setRotation(gle::Vector3<GLfloat>(0, 1, 0), 45);
  // 	      scene << cube;
  // 	    }
  // 	}
  //   }

  gle::Material materialLight;

  materialLight.setDiffuseLightEnabled(false);
  materialLight.setSpecularLightEnabled(false);
  gle::PointLight l(gle::Vector3<GLfloat>(0, 20, 0), gle::Color<GLfloat>(0.8, 0.8, 0.8));
  gle::Mesh* sp = gle::Geometries::Sphere(&materialLight, dist/100);
  sp->setPosition(gle::Vector3<GLfloat>(0, 20, 0));
  gle::PointLight l2(gle::Vector3<GLfloat>(0, 0, -20), gle::Color<GLfloat>(0.8, 0.8, 0.8));
  gle::Mesh* sp2 = gle::Geometries::Sphere(&materialLight, dist/100);
  sp2->setPosition(gle::Vector3<GLfloat>(0, 0, -20));
  gle::PointLight l3(gle::Vector3<GLfloat>(40, 0, 0), gle::Color<GLfloat>(0.8, 0.8, 0.8));
  gle::Mesh* sp3 = gle::Geometries::Sphere(&materialLight, dist/100);
  sp3->setPosition(gle::Vector3<GLfloat>(40, 0, 0));

  scene << &camera << sp << sp2 << sp3;
  scene << &l << &l2 << &l3;

  gle::Renderer renderer;

  GLfloat angle = 0;

  sf::Clock clock;
  sf::Clock time;

  size_t frameCounter = 0;
  
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
      if (helice)
	helice->setRotation(gle::Vector3<GLfloat>(1, 0, 0), angle * -1000);
      camera.setPosition(gle::Vector3<GLfloat>(cos(angle/3) * -dist,
					       0,//sin(angle/3) * dist/2,
					       sin(angle/3) * -dist));

      float dist2 = dist / 3;
      sp->setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, cos(angle/2) * dist2, 0));
      l.setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, cos(angle/2) * dist2, 0));

      sp2->setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, 0, cos(angle/2) * dist2));
      l2.setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, 0, cos(angle/2) * dist2));

      sp3->setPosition(gle::Vector3<GLfloat>(0, sin(angle/2) * dist2, cos(angle/2) * dist2));
      l3.setPosition(gle::Vector3<GLfloat>(0, sin(angle/2) * dist2, cos(angle/2) * dist2));

      scene.updateLights();

      renderer.render(&scene);
      App.display();
    }
  
  return (0);
}
