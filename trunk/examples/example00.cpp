//
// main.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 15 17:24:36 2012 gael jochaud-du-plessix
// Last update Wed Feb 29 11:47:10 2012 loick michard
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
  if (ac < 3)
    return (0);
  sf::ContextSettings context;
  context.DepthBits = 24;
  context.StencilBits = 24;
  context.AntialiasingLevel = 2;
  context.MajorVersion = 3;
  context.MinorVersion = 3;

  sf::Window App(sf::VideoMode(500, 500, 32), "glEngine",
		 sf::Style::Default, context);
  
  //! Print OpenGL supported version
  context = App.GetSettings();
  std::cout << context.MajorVersion << '.'
	    << context.MinorVersion << std::endl;

  App.SetActive();

  gle::Scene scene;
  gle::PerspectiveCamera camera(gle::Vector3<GLfloat>(0, 0, 0),
				gle::Vector3<GLfloat>(0, 0, 0),
				45, 1, 1, 10000);
  gle::Material material;

  material.setColorEnabled(false);
  material.setShininess(32);

  gle::ObjLoader loader;
  gle::Mesh* model = loader.load(av[1]);

  GLfloat dist = atol(av[2]);

  gle::Mesh* helice = NULL;

  srand(time(NULL));

  if (model)
    {
      std::vector<gle::Mesh*> childs = model->getChildren();
      for (std::vector<gle::Mesh*>::iterator it = childs.begin();
	   it != childs.end(); ++it)
	{
	  gle::Mesh* mesh = *it;
	  gle::Color<GLfloat> color((double)random() / RAND_MAX,
				    (double)random() / RAND_MAX,
				    (double)random() / RAND_MAX, 1.0);
	  gle::Array<GLfloat> colors(4 * mesh->getNbIndexes());
	  for (int i = 0; i < mesh->getNbIndexes(); i++)
	    colors.push(color);
	  if (mesh->getName() == "PlanePropeller_mesh")
	    helice = mesh;
	  mesh->setColors(colors);
	  mesh->setMaterial(&material);
	  scene << mesh;
	}
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

  materialLight.isLight(true);
  gle::PointLight l(gle::Vector3<GLfloat>(0, 20, 0), gle::Color<GLfloat>(0.8, 0, 0));
  gle::Mesh* sp = gle::Geometries::Sphere(&materialLight, dist/100, gle::Color<GLfloat>(0.8, 0, 0));
  sp->setPosition(gle::Vector3<GLfloat>(0, 20, 0));
  gle::PointLight l2(gle::Vector3<GLfloat>(0, 0, -20), gle::Color<GLfloat>(0, 0, 0.8));
  gle::Mesh* sp2 = gle::Geometries::Sphere(&materialLight, dist/100, gle::Color<GLfloat>(0, 0, 0.8));
  sp2->setPosition(gle::Vector3<GLfloat>(0, 0, -20));
  gle::PointLight l3(gle::Vector3<GLfloat>(40, 0, 0), gle::Color<GLfloat>(0, 0.8, 0));
  gle::Mesh* sp3 = gle::Geometries::Sphere(&materialLight, dist/100, gle::Color<GLfloat>(0, 0.8, 0));
  sp3->setPosition(gle::Vector3<GLfloat>(40, 0, 0));

  scene << &camera << &material << &materialLight << sp << sp2 << sp3;
  scene << &l << &l2 << &l3;
  scene.setLightEnabled(true, gle::Color<GLfloat>(0, 0, 0));

  gle::Renderer renderer;
  renderer.createPrograms(&scene);

  GLfloat angle = 0;

  sf::Clock clock;
  sf::Clock time;

  size_t frameCounter = 0;
  
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
      App.Display();
    }
  
  return (0);
}
